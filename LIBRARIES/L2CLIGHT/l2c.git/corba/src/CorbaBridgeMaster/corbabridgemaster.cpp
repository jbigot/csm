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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <omniORB4/CORBA.h>
#include <stdint.h>

#include <collective.h>
#include <corbaaccess.h>
#include <initializer.h>
#include <interbridge.h>
#include <llcmcpp3.h>
#include <rawcomponentmanagement.h>
#include <master.h>
#include <slave.h>

using ::boost::condition_variable;
using ::boost::lock_guard;
using ::boost::mutex;
using ::boost::unique_lock;
using ::CORBA::Object;
using ::CORBA::Object_ptr;
using ::CORBA::ORB_init;
using ::CORBA::ORB_ptr;
using ::PortableServer::POA;
using ::PortableServer::POA_var;
using ::std::abort;
using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::list;
using ::std::make_pair;
using ::std::map;
using ::std::ofstream;
using ::std::string;
using ::std::stringstream;

namespace llcmcpp {

using namespace POA_llcmcpp;

namespace impl {

class CorbaBridgeMaster:
		virtual public Collective,
		virtual public CorbaAccess,
		virtual public Initializer,
		virtual private POA_llcmcpp::Master
{
public:
	class Initializer2:
			virtual public Initializer
	{
	public:
		Initializer2 ( CorbaBridgeMaster& container ):
				m_container(container)
		{}
		
		virtual void initialize ( int* argc, char*** argv )
		{
			m_container.initialize2(argc, argv);
		}
		
	private:
		CorbaBridgeMaster& m_container;
		
	} initializer2; // Ugly !!! but IOR must be broadcasted before rendezvous
	
	string ior_file;
	
	string* local_host;
	
	string master_ior;
	
	ORB_ptr orb;
	
	RawComponentManagement* raw_component_management;
	
	void nbpeers ( string nbpeers )
	{
		stringstream valstr(nbpeers);
		valstr >> m_nbpeers;
		if ( ! valstr ) {
			cerr << "[CorbaBridgeMaster] ** ERROR: Invalid nbpeers: " << nbpeers << endl;
			abort();
		}
		m_barrier_count = m_nbpeers + 1;
	}
	
	CorbaBridgeMaster ():
			initializer2(*this),
			ior_file("/tmp/cam.ior"),
			local_host(0),
			raw_component_management(0),
			m_barrier_count(1),
			m_inter_bridge(raw_component_management),
			m_nbpeers(0)
	{}
	
	~CorbaBridgeMaster ()
	{
		POA_var poa = POA::_narrow(orb->resolve_initial_references("RootPOA"));
		poa->the_POAManager()->deactivate(true, true);
	}
	
	virtual void initialize ( int* argc, char*** argv )
	{
		orb = ORB_init(*argc, *argv);
		POA_var poa = POA::_narrow(orb->resolve_initial_references("RootPOA"));
		poa->the_POAManager()->activate();
		master_ior = orb->object_to_string(_this());
		cout << "CorbaAssemblyManagement: " << master_ior << endl;
		if ( !ior_file.empty() ) {
			ofstream of(ior_file.c_str());
			of << master_ior;
		}
	}
	
	virtual void initialize2 ( int* argc, char*** argv )
	{
		unique_lock< mutex > lock(m_mutex);
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] waiting at rendezvous" << endl;
#endif // NDEBUG
		m_peers.insert(make_pair(*local_host, m_inter_bridge._this()));
		while ( m_peers.size() != m_nbpeers+1 ) m_barrier_condition.wait(lock);
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] leaving rendezvous" << endl;
#endif // NDEBUG
	}
	
	virtual void bridge_register ( InterBridge_ptr as_bridge, Slave_ptr as_slave, const char* peer_name )
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] bridge_register" << endl;
#endif // NDEBUG
		lock_guard< mutex > lock(m_mutex);
		m_peers.insert(make_pair(peer_name, llcmcpp::InterBridge::_duplicate(as_bridge)));
		m_slaves.push_back(Slave::_duplicate(as_slave));
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] one peer at rendezvous: "<<peer_name<<" (" << m_peers.size() << '/' << m_nbpeers+1 << ')' << endl;
#endif // NDEBUG
		if ( m_peers.size() == m_nbpeers+1 ) {
#ifndef NDEBUG
			cerr << "[CorbaBridgeMaster] last peer at rendezvous" << endl;
#endif // NDEBUG
			for ( list< Slave_ptr >::iterator slit = m_slaves.begin(); slit != m_slaves.end(); ++slit ) {
				map< string, InterBridge_ptr >::iterator peit = m_peers.begin();
				while ( peit != m_peers.end() ) {
					const char* peer_name = peit->first.c_str();
					InterBridge_ptr peer = peit->second;
					++peit;
					bool last = ( peit == m_peers.end() );
					(*slit)->bridge_register(peer, peer_name, last);
				}
			}
			m_barrier_condition.notify_all();
#ifndef NDEBUG
			cerr << "[CorbaBridgeMaster] all peers notified" << endl;
#endif // NDEBUG
		}
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] let's leave" << endl;
#endif // NDEBUG
	}
	
	virtual void enter_barrier()
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] enter_barrier" << endl;
#endif // NDEBUG
		lock_guard< mutex > lock(m_mutex);
		if ( --m_barrier_count == 0 ) {
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] enter_barrier: he was last!" << endl;
#endif // NDEBUG
			m_barrier_condition.notify_all();
		}
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] leaving enter_barrier" << endl;
#endif // NDEBUG
	}

	virtual void barrier ()
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] barrier" << endl;
#endif // NDEBUG
		unique_lock< mutex > lock(m_mutex);
		
		// wait for everyone
		--m_barrier_count;
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] barrier: still waiting for " << m_barrier_count << endl;
#endif // NDEBUG
		while ( m_barrier_count ) m_barrier_condition.wait(lock);
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] barrier: re-awakening" << endl;
#endif // NDEBUG
		
		m_barrier_count = m_nbpeers + 1;
		
		for ( list< Slave_ptr >::iterator it = m_slaves.begin(); it != m_slaves.end(); ++it ) {
			(*it)->leave_barrier();
		}
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] leaving barrier" << endl;
#endif // NDEBUG
	}
	
	virtual Object_ptr get ( string host, EndPoint property )
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeMaster] get" << endl;
#endif // NDEBUG
		lock_guard< mutex > lock(m_mutex);
		return m_peers.at(host)->get(property.instance.c_str(), property.port.c_str());
	}
	
private:
	condition_variable m_barrier_condition;
	
	unsigned int m_barrier_count;
	
	class InterBridge:
			virtual public POA_llcmcpp::InterBridge
	{
	public:
		InterBridge ( RawComponentManagement*& raw_component_management ):
				m_raw_component_management(raw_component_management)
		{}
		
		virtual Object_ptr get ( const char* componentId, const char* portname )
		{
			return static_cast< Object_ptr >(m_raw_component_management->get(EndPoint(componentId, portname)));
		}
		
	private:
		RawComponentManagement*& m_raw_component_management;
		
	} m_inter_bridge;
	
	mutex m_mutex;
	
	uint16_t m_nbpeers;
	
	map< string, InterBridge_ptr > m_peers;
	
	list< Slave_ptr > m_slaves;
	
};

LCMP(CorbaBridgeMaster)
	L_PROPERTY(string, ior_file);
	L_PROPERTY(string, nbpeers);
	L_CPP_PROVIDE(Collective, collective)
	L_CPP_PROVIDE(CorbaAccess, corba);
	L_CPP_PROVIDE(Initializer, initializer);
	L_CPP_PROVIDE2(Initializer, initializer2);
	L_CPP_PROVIDE2(string, master_ior);
	L_CPP_PROVIDE2(ORB_ptr, orb);
	L_CPP_USE(string, local_host);
	L_CPP_USE(RawComponentManagement, raw_component_management);
LEND

}

}
