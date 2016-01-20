/*
    This file is part of LLCMc++ (a.k.a. llcmcpp)
    Copyright (C) 2011 INRIA Julien Bigot <julien.bigot@inria.fr>
    Copyright (C) 2011 INRIA Christian Pérez <christian.perez@inria.fr>

    LLCMc++ is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LLCMc++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LLCMc++.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <list>

#include <llcmcpp/hostid.h>
#include <llcmcpp/initializer.h>
#include <llcmcpp/llcmcpp3.h>

#include <asynccollective.h>

#include <CharmCollective.decl.h>

using ::llcmcpp::HostId;
using ::llcmcpp::Initializer;
using ::std::cerr;
using ::std::endl;
using ::std::list;

namespace gluon {

namespace impl {

class CharmCollective:
		virtual public AsyncCollective,
		virtual public Initializer
{
public:
	HostId local_rank;
	
	CharmCollective ():
			local_rank(CkMyPe()),
			m_backbone_access(0),
			m_called_barrier(0)
	{
		_registerCharmCollective();
		s_waiting_collectives.push_back(this);
	}
	
	static void set_backbone ( CollectiveBackbone* backbone );
	
	virtual void initialize ( int* argc, char*** argv )
	{
#ifndef NDEBUG
		cerr << "[CharmCollective|"<<CkMyPe()<<"] initialize()"<<endl;
#endif
		if ( CkMyPe() == 0 ) CProxy_CollectiveBackbone::ckNew();
	}
	
	virtual void barrier ( Callback& cb )
	{
		if ( m_backbone_access ) m_backbone_access->barrier(cb);
		else m_called_barrier = &cb;
	}
	
private:
	AsyncCollective* m_backbone_access;
	
	Callback* m_called_barrier;
	
	static list< CharmCollective* > s_waiting_collectives;
};

LCMP(CharmCollective)
	L_CPP_PROVIDE(AsyncCollective, collective);
	L_CPP_PROVIDE(Initializer, init);
	L_CPP_PROVIDE2(HostId, local_rank);
LEND

}

}

// inspired by Hensgen, Finkel, and Manber's dissemination barrier
class CollectiveBackbone:
		public CBase_CollectiveBackbone,
		virtual public gluon::AsyncCollective
{
public:
	static void init_nb_rounds ()
	{
		s_nb_rounds = 0;
		unsigned long value = 1;
		while ( value < CkNumPes() ) {
			++s_nb_rounds;
			value <<= 1;
		}
		++s_nb_rounds; // one for free for self signaling
	}
	
	CollectiveBackbone ():
		m_barrier_id(0),
		m_barrier_round(0),
		m_barrier_callback(0)
	{
#ifndef NDEBUG
		cerr << "[CollectiveBackbone|"<<CkMyPe()<<"] CollectiveBackbone()"<<endl;
#endif
		m_barrier_flags[0] = new unsigned short[s_nb_rounds];
		for ( size_t ii=0; ii<s_nb_rounds; ++ii ) m_barrier_flags[0][ii] = -1;
		m_barrier_flags[1] = new unsigned short[s_nb_rounds];
		for ( size_t ii=0; ii<s_nb_rounds; ++ii ) m_barrier_flags[1][ii] = -1;
		gluon::impl::CharmCollective::set_backbone(this);
	}
	
	void barrier ( gluon::Callback& cb )
	{
		m_barrier_callback = &cb;
		signal(0, m_barrier_id);
	}
	
	static size_t peer ( size_t round ) { return (CkMyPe()+(1<<(round-1)))%CkNumPes(); }
	
	void signal ( unsigned round, unsigned short id )
	{
		m_barrier_flags[(id&1)][round] = (id>>1); // clear the round we've just been signaled about
#ifndef NDEBUG
		cerr << "[CharmCollective|"<<CkMyPe()<<"] received signal for round "<<(round+1)<<"/"<<s_nb_rounds<<", id "<<id<<endl;
#endif
		
		while ( m_barrier_round < s_nb_rounds && m_barrier_flags[m_barrier_id&1][m_barrier_round] == (m_barrier_id>>1)  )  { // if the current round is valid & cleared
#ifndef NDEBUG
			cerr << "[CharmCollective|"<<CkMyPe()<<"] ok, m_barrier_flags["<<(m_barrier_id&1)<<"]["<<m_barrier_round<<"] == "<<(m_barrier_id>>1)<<endl;
#endif
			
			++m_barrier_round; //move to the next round
#ifndef NDEBUG
			cerr << "[CharmCollective|"<<CkMyPe()<<"] entering round "<<(m_barrier_round+1)<<"/"<<s_nb_rounds<<endl;
#endif
			
			if ( m_barrier_round < s_nb_rounds ) { // if the round we just entered is still valid
				thisProxy[peer(m_barrier_round)].signal(m_barrier_round, m_barrier_id); // signal this round
#ifndef NDEBUG
				cerr << "[CharmCollective|"<<CkMyPe()<<"] signal sent to "<<peer(m_barrier_round)<<endl;
#endif
			}
		}
		if ( m_barrier_round == s_nb_rounds ) { // if we crossed the barrier
			if (!CkMyPe()) CkStartQD(CkCallback(CkIndex_CollectiveBackbone::barrier_crossed(), thisProxy));
		}
	}
	
	void barrier_crossed ()
	{
		++m_barrier_id;
		m_barrier_round=0;
		gluon::Callback& cb = *m_barrier_callback;
		m_barrier_callback = 0;
#ifndef NDEBUG
		cerr << "[CharmCollective|"<<CkMyPe()<<"] barrier crossed, calling callback!"<<endl;
#endif
		cb();
	}
	
private:
	static unsigned s_nb_rounds; ///< how many rounds we have to do in each barrier == ceiling(log2(nbprocess))+1
	
	unsigned m_barrier_round; ///< the round we are waiting the signal for (the next one we're going to signal)
	gluon::Callback* m_barrier_callback; ///< the function we'll call once the barrier is crossed
	unsigned short m_barrier_id; ///< the id of the current barrier
	/** barrier flags
	 * this is a 2 dimensional array m_barrier_flags[2][m_log_nbprocess+1]
	 * the first dimension is the lowest bit of the barrier id because we can receive signal for the next barrier while we diddn't leave the current one yet
	 * the second dimension is for the round, in addition to the standard dissemination barrier, round 0 is a self signaling round, then round 1...m_log_nbprocess are the normal rounds
	 * a round is cleared if m_barrier_flags[m_barrier_id&1][m_barrier_round] == (m_barrier_id>>1) this is done in order not to have to reset flags at each iteration.
	 */
	unsigned short *(m_barrier_flags[2]);
	
};

unsigned CollectiveBackbone::s_nb_rounds = 0;

#include <CharmCollective.def.h>

list< gluon::impl::CharmCollective* > gluon::impl::CharmCollective::s_waiting_collectives;

void
gluon::impl::CharmCollective::set_backbone ( CollectiveBackbone* backbone )
{
	CharmCollective* initialized = s_waiting_collectives.front();
	s_waiting_collectives.pop_front();
	initialized->m_backbone_access = backbone;
	Callback* called_barrier = initialized->m_called_barrier;
	initialized->m_called_barrier = 0;
	if ( called_barrier ) backbone->barrier(*called_barrier);
}
