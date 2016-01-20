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

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

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
using ::CORBA::Boolean;
using ::CORBA::Object;
using ::CORBA::Object_ptr;
using ::CORBA::ORB_init;
using ::CORBA::ORB_ptr;
using ::PortableServer::POA;
using ::PortableServer::POA_var;
using ::std::cerr;
using ::std::endl;
using ::std::map;
using ::std::string;

namespace llcmcpp {

namespace impl {

class CorbaBridgeSlave:
		virtual public Collective,
		virtual public CorbaAccess,
		virtual public Initializer,
		virtual private POA_llcmcpp::Slave
{
public:
	string* local_host;
	
	RawComponentManagement* raw_component_management;
	
	string master_ior;
	
	ORB_ptr orb;
	
	CorbaBridgeSlave ():
			local_host(0),
			m_inter_bridge(raw_component_management)
	{}
	
	~CorbaBridgeSlave ()
	{
		POA_var poa = POA::_narrow(orb->resolve_initial_references("RootPOA"));
		poa->the_POAManager()->deactivate(true, true);
	}
	
	virtual void initialize ( int* argc, char*** argv )
	{
		unique_lock< mutex > lock(m_mutex);
		
		orb = ORB_init(*argc, *argv);
		POA_var poa = POA::_narrow(orb->resolve_initial_references("RootPOA"));
		poa->the_POAManager()->activate();
		
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] going to rendezvous";
#endif // NDEBUG
		
		m_master = Master::_narrow(orb->string_to_object(master_ior.c_str()));
#ifndef NDEBUG
		cerr << " ..." << endl;
#endif // NDEBUG
		m_barrier_locked = true;
		lock.unlock();
		m_master->bridge_register(m_inter_bridge._this(), _this(), local_host->c_str());
		lock.lock();
		
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] waiting at rendezvous" << endl;
#endif // NDEBUG
		
		while ( m_barrier_locked ) m_barrier_condition.wait(lock);
		
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] leaving rendezvous" << endl;
#endif // NDEBUG
	}
	
	virtual void barrier ()
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] barrier" << endl;
#endif // NDEBUG
		unique_lock< mutex > lock(m_mutex);
		
		m_master->enter_barrier();
		
		m_barrier_locked = true;
		while ( m_barrier_locked ) m_barrier_condition.wait(lock);
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] leaving barrier" << endl;
#endif // NDEBUG
	}
	
	virtual Object_ptr get ( string host, EndPoint property )
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] get("<<host<<", ("<<property.instance<<", "<<property.port<<")"<<endl;
#endif // NDEBUG
		return m_peers.at(host)->get(property.instance.c_str(), property.port.c_str());
	}
	
private:
	virtual void bridge_register ( InterBridge_ptr other, const char* name, Boolean last )
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] bridge_register" << endl;
#endif // NDEBUG
		unique_lock< mutex > lock(m_mutex);
		
		m_peers[name] = llcmcpp::InterBridge::_duplicate(other);
		
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] joined at rendezvous" << endl;
#endif // NDEBUG
		
		if ( last ) {
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] last but not least" << endl;
#endif // NDEBUG
			m_barrier_locked = false;
			m_barrier_condition.notify_all();
		}
	}
	
	virtual void leave_barrier()
	{
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] leave_barrier" << endl;
#endif // NDEBUG
		unique_lock< mutex > lock(m_mutex);
		m_barrier_locked = false;
		m_barrier_condition.notify_all();
#ifndef NDEBUG
		cerr << "[CorbaBridgeSlave] leaving leave_barrier" << endl;
#endif // NDEBUG
	}
	
	class InterBridge:
			virtual public POA_llcmcpp::InterBridge
	{
	public:
		InterBridge ( RawComponentManagement*& raw_component_management ):
				m_raw_component_management(raw_component_management)
		{}
		
		virtual Object_ptr get ( const char* componentId, const char* portname )
		{
#ifndef NDEBUG
			cerr << "[CorbaBridgeSlave:InterBridge] get(" << componentId << ", " << portname << ')' << endl;
#endif // NDEBUG
			return static_cast< Object_ptr >(m_raw_component_management->get(EndPoint(componentId, portname)));
		}
		
	private:
		RawComponentManagement*& m_raw_component_management;
		
	} m_inter_bridge;
	
	mutex m_mutex;
	
	condition_variable m_barrier_condition;
	
	bool m_barrier_locked;
	
	map< string, InterBridge_ptr > m_peers;
	
	Master_ptr m_master;
	
};

LCMP(CorbaBridgeSlave)
	L_PROPERTY(string, master_ior);
	L_CPP_PROVIDE(Collective, collective);
	L_CPP_PROVIDE(CorbaAccess, corba);
	L_CPP_PROVIDE(Initializer, initializer);
	L_CPP_PROVIDE2(string, master_ior);
	L_CPP_PROVIDE2(ORB_ptr, orb);
	L_CPP_USE(string, local_host);
	L_CPP_USE(RawComponentManagement, raw_component_management);
LEND

}

}

