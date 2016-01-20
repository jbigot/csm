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
#include <string>

#include <llcmcpp/clusterplacementmanagement.h>
#include <llcmcpp/endpoint.h>
#include <llcmcpp/initializer.h>
#include <llcmcpp/clusterinstantiationmanagement.h>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/localinstantiationmanagement.h>
#include <llcmcpp/namespacemanagement.h>
#include <llcmcpp/rawcomponentmanagement.h>
#include <llcmcpp/rmimanagement.h>

#include <charminstantiationmanagement.h>

#include <charmmanager.decl.h>

namespace gluon {

namespace impl {

using ::llcmcpp::ClusterInstantiationManagement;
using ::llcmcpp::ClusterPlacementManagement;
using ::llcmcpp::EndPoint;
using ::llcmcpp::HostId;
using ::llcmcpp::Initializer;
using ::llcmcpp::LocalInstantiationManagement;
using ::llcmcpp::NamespaceManagement;
using ::llcmcpp::RawComponentManagement;
using ::llcmcpp::RmiManagement;
using ::std::string;
using ::std::cerr;
using ::std::endl;
using ::std::list;

class CharmManager:
		virtual public RmiManagement,
		virtual public Initializer,
		virtual public CharmInstantiationManagement
{
public:
	ClusterInstantiationManagement* instantiation_management;
	
	NamespaceManagement* global_namespace_management;
	
	LocalInstantiationManagement* local_instantiation_management;
	
	NamespaceManagement* local_namespace_management;
	
	ClusterPlacementManagement* namespace_management;
	
	RawComponentManagement* raw_component_management;
	
	CharmManager ():
			instantiation_management(0),
			global_namespace_management(0),
			local_instantiation_management(0),
			local_namespace_management(0),
			namespace_management(0),
			raw_component_management(0),
			charm_array_manager(*this),
			charm_group_manager(*this),
			m_backbone_access(0),
			m_rid(0)
	{
		_registercharmmanager();
		s_waiting_managers.push_back(this);
	}
	
	static void set_backbone ( ManagerBackbone* backbone );
	
	void initialize ( int* argc, char*** argv )
	{
#ifndef NDEBUG
		cerr << "[CharmManager|"<<CkMyPe()<<"] initialize()"<<endl;
#endif
		if ( CkMyPe() == 0 ) CProxy_ManagerBackbone::ckNew();
	}
	
	void connect ( EndPoint user, EndPoint provider );
	
	void set ( EndPoint user, CkChareID value )
	{
		raw_component_management->set(user, &value);
	}
	
	void connect_group ( EndPoint user, EndPoint provider );
	
	void set_group ( EndPoint user, CkGroupID value )
	{
		raw_component_management->set(user, &value);
	}
	
	void connect_array ( EndPoint user, EndPoint provider );
	
	void set_array ( EndPoint user, CkArrayID value )
	{
		raw_component_management->set(user, &value);
	}
	
	void instantiate_array ( string name, uint32_t size, string type, string from );
	
	void instantiate_bound_array ( std::string name, uint32_t size, std::string bound_to, std::string type, std::string from  );
	
	void instantiate_group ( string name, string type, string from );
	
	class CharmArrayManager:
			virtual public RmiManagement
	{
	public:
		CharmArrayManager ( CharmManager& container ):
				m_container(container)
		{
		}
		
		void connect ( EndPoint user, EndPoint provider )
		{
			m_container.connect_array(user, provider);
		}
		
	private:
		CharmManager& m_container;
		
	} charm_array_manager;
	
	class CharmGroupManager:
			virtual public RmiManagement
	{
	public:
		CharmGroupManager ( CharmManager& container ):
				m_container(container)
		{
		}
		
		void connect ( EndPoint user, EndPoint provider )
		{
			m_container.connect_group(user, provider);
		}
		
	private:
		CharmManager& m_container;
		
	} charm_group_manager;
	
private:
	ManagerBackbone* m_backbone_access;
	
	uint32_t m_rid;
	
	struct CalledGroupInstantiate
	{
		string name;
		string type;
		string from;
		CalledGroupInstantiate ( string name, string type, string from ):
				name(name),
				type(type),
				from(from)
		{}
	};
	
	list< CalledGroupInstantiate > m_called_group_instantiate;
	
	struct CalledConnect
	{
		EndPoint user;
		EndPoint provider;
		CalledConnect ( EndPoint user, EndPoint provider ):
				user(user),
				provider(provider)
		{}
	};
	
	list< CalledConnect > m_called_connects;
	
	list< CalledConnect > m_called_array_connects;
	
	list< CalledConnect > m_called_group_connects;
	
	static list< CharmManager* > s_waiting_managers;
};

LCMP(CharmManager)
	L_CPP_PROVIDE(CharmInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(RmiManagement, charm_manager);
	L_CPP_PROVIDE2(RmiManagement, charm_array_manager);
	L_CPP_PROVIDE2(RmiManagement, charm_group_manager);
	L_CPP_PROVIDE(Initializer, init);
	L_CPP_USE(ClusterInstantiationManagement, instantiation_management);
	L_CPP_USE(NamespaceManagement, global_namespace_management);
	L_CPP_USE(LocalInstantiationManagement, local_instantiation_management);
	L_CPP_USE(NamespaceManagement, local_namespace_management);
	L_CPP_USE(ClusterPlacementManagement, namespace_management);
	L_CPP_USE(RawComponentManagement, raw_component_management);
LEND

}

}

class ManagerBackbone:
		public CBase_ManagerBackbone
{
public:
	ManagerBackbone ()
	{
		gluon::impl::CharmManager::set_backbone(this);
	}
	
	llcmcpp::RawComponentManagement* raw_component_management;
	
	std::list< uint32_t > m_waiting_id;
	
	template< class CompoID >
	struct WaitingSet
	{
		llcmcpp::EndPoint property;
		CompoID value;
		WaitingSet(llcmcpp::EndPoint property, CompoID value):
				property(property),
				value(value)
		{}
	};
	
	typedef std::map< uint32_t, WaitingSet< CkChareID > > waiting_set_t;
	waiting_set_t m_waiting_set;
	
	typedef std::map< uint32_t, WaitingSet< CkArrayID > > waiting_array_set_t;
	waiting_array_set_t m_waiting_array_set;
	
	typedef std::map< uint32_t, WaitingSet< CkGroupID > > waiting_group_set_t;
	waiting_group_set_t m_waiting_group_set;
	
	void wid ( uint32_t rid )
	{
		m_waiting_id.push_back(rid);
		pop_waiting();
	}
	
	void pop_waiting ()
	{
		while ( !m_waiting_id.empty() ) {
			uint32_t rid = m_waiting_id.front();
			// try to handle this id as a simple request
			waiting_set_t::iterator cit = m_waiting_set.find(rid);
			if ( cit != m_waiting_set.end() ) {
#ifndef NDEBUG
				std::cerr << "[ManagerBackbone|"<<CkMyPe()<<"] set["<<rid<<"] ! "<<cit->second.property.instance<<"."<<cit->second.property.port<<" = "<<cit->second.value.objPtr<<std::endl;
#endif
				raw_component_management->set(cit->second.property, &(cit->second.value));
				m_waiting_set.erase(cit);
				m_waiting_id.pop_front();
				continue;
			}
			// try to handle this id as an array request
			waiting_array_set_t::iterator ait = m_waiting_array_set.find(rid);
			if ( ait != m_waiting_array_set.end() ) {
#ifndef NDEBUG
				std::cerr << "[ManagerBackbone|"<<CkMyPe()<<"] set_array["<<rid<<"] ! "<<ait->second.property.instance<<"."<<ait->second.property.port<<" = "<<static_cast<CkGroupID>(ait->second.value).idx<<std::endl;
#endif
				raw_component_management->set(ait->second.property, &(ait->second.value));
				m_waiting_array_set.erase(ait);
				m_waiting_id.pop_front();
				continue;
			}
			// try to handle this id as a group request
			waiting_group_set_t::iterator git = m_waiting_group_set.find(rid);
			if ( git != m_waiting_group_set.end() ) {
#ifndef NDEBUG
				std::cerr << "[ManagerBackbone|"<<CkMyPe()<<"] set_group["<<rid<<"] ! "<<git->second.property.instance<<"."<<git->second.property.port<<" = "<<git->second.value.idx<<std::endl;
#endif
				raw_component_management->set(git->second.property, &(git->second.value));
				m_waiting_group_set.erase(git);
				m_waiting_id.pop_front();
				continue;
			}
			// if the first request is not waiting, abort for now
			return;
		}
	}
	
	void set ( unsigned uis, char* ui, unsigned ups, char* up, CkChareID value, uint32_t rid )
	{
#ifndef NDEBUG
		std::cerr << "[ManagerBackbone|"<<CkMyPe()<<"] set_array["<<rid<<"]: "<<ui<<"."<<up<<" = "<<value.objPtr<<std::endl;
#endif
		m_waiting_set.insert(std::make_pair(rid, WaitingSet< CkChareID >(llcmcpp::EndPoint(std::string(ui, uis-1), std::string(up, ups-1)), value)));
		pop_waiting();
	}
	
	void set_array ( unsigned uis, char* ui, unsigned ups, char* up, CkArrayID value, uint32_t rid )
	{
#ifndef NDEBUG
		std::cerr << "[ManagerBackbone|"<<CkMyPe()<<"] set_array["<<rid<<"]: "<<ui<<"."<<up<<" = "<<static_cast<CkGroupID>(value).idx<<std::endl;
#endif
		m_waiting_array_set.insert(std::make_pair(rid, WaitingSet< CkArrayID >(llcmcpp::EndPoint(std::string(ui, uis-1), std::string(up, ups-1)), value)));
		pop_waiting();
	}
	
	void set_group ( unsigned uis, char* ui, unsigned ups, char* up, CkGroupID value, uint32_t rid )
	{
#ifndef NDEBUG
		std::cerr << "[ManagerBackbone|"<<CkMyPe()<<"] set_array["<<rid<<"]: "<<ui<<"."<<up<<" = "<<value.idx<<std::endl;
#endif
		m_waiting_group_set.insert(std::make_pair(rid, WaitingSet< CkGroupID >(llcmcpp::EndPoint(std::string(ui, uis-1), std::string(up, ups-1)), value)));
		pop_waiting();
	}
	
};

#include <charmmanager.def.h>

std::list< gluon::impl::CharmManager* > gluon::impl::CharmManager::s_waiting_managers;

void
gluon::impl::CharmManager::set_backbone ( ManagerBackbone* backbone )
{
	CharmManager* initialized = s_waiting_managers.front();
	s_waiting_managers.pop_front();
	initialized->m_backbone_access = backbone;
	backbone->raw_component_management = initialized->raw_component_management;
	
	for ( list< CalledGroupInstantiate >::iterator it = initialized->m_called_group_instantiate.begin(); it != initialized->m_called_group_instantiate.end(); ++it ) {
		initialized->instantiate_group(it->name, it->type, it->from);
	}
	initialized->m_called_group_instantiate.clear();
	
	for ( list< CalledConnect >::iterator it = initialized->m_called_connects.begin(); it != initialized->m_called_connects.end(); ++it ) {
		initialized->connect(it->user, it->provider);
	}
	initialized->m_called_connects.clear();
	for ( list< CalledConnect >::iterator it = initialized->m_called_array_connects.begin(); it != initialized->m_called_array_connects.end(); ++it ) {
		initialized->connect_array(it->user, it->provider);
	}
	initialized->m_called_array_connects.clear();
	for ( list< CalledConnect >::iterator it = initialized->m_called_group_connects.begin(); it != initialized->m_called_group_connects.end(); ++it ) {
		initialized->connect_group(it->user, it->provider);
	}
	initialized->m_called_group_connects.clear();
}

void
gluon::impl::CharmManager::instantiate_array ( string name, uint32_t size, string type, string from )
{
	instantiation_management->instantiate(0, name, type, from);
	if ( CkMyPe() == 0 ) {
		raw_component_management->set(EndPoint(name, "__llcmcpp__gluon__arraysize"), &size);
		delete static_cast<CkArrayID*>(raw_component_management->get(EndPoint(name, "__llcmcpp__gluon__arrayid")));
	}
}

void
gluon::impl::CharmManager::instantiate_bound_array ( string name, uint32_t size, std::string bound_to, string type, string from )
{
	instantiation_management->instantiate(0, name, type, from);
	if ( CkMyPe() == 0 ) {
		raw_component_management->set(EndPoint(name, "__llcmcpp__gluon__arraysize"), &size);
		CkArrayID* bound_id = static_cast<CkArrayID*>(raw_component_management->get(EndPoint(bound_to, "__llcmcpp__gluon__arrayid")));
		raw_component_management->set(EndPoint(name, "__llcmcpp__gluon__arraybind"), bound_id);
		delete bound_id;
		delete static_cast<CkArrayID*>(raw_component_management->get(EndPoint(name, "__llcmcpp__gluon__arrayid")));
	}
}

void
gluon::impl::CharmManager::instantiate_group ( string name, string type, string from )
{
	if ( m_backbone_access ) {
		m_backbone_access->wid(++m_rid);
		local_instantiation_management->instantiate(name, type, from);
		if ( CkMyPe() == 0 ) {
			CkGroupID* id = static_cast<CkGroupID*>(raw_component_management->get(EndPoint(name, "__llcmcpp__gluon__groupid")));
			m_backbone_access->thisProxy.set_group(name.size()+1, const_cast<char*>(name.c_str()), 26, "__llcmcpp__gluon__groupid", *id, m_rid);
			delete id;
		}
	} else {
		m_called_group_instantiate.push_back(CalledGroupInstantiate(name, type, from));
	}
}

void
gluon::impl::CharmManager::connect ( EndPoint user, EndPoint provider )
{
	if ( m_backbone_access ) {
		++m_rid;
		if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
			m_backbone_access->wid(m_rid);
		}
		// the connector local to 	the provider is responsible for connection
		if ( local_namespace_management->instances().find(provider.instance) != local_namespace_management->instances().end() ) {
			CkChareID* cid =  static_cast<CkChareID*>(raw_component_management->get(provider));
			if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
				m_backbone_access->thisProxy.ckLocalBranch()->set(user.instance.size()+1, const_cast<char*>(user.instance.c_str()), user.port.size()+1, const_cast<char*>(user.port.c_str()), *cid, m_rid);
			} else {
				m_backbone_access->thisProxy[namespace_management->get_rank(user.instance)].set(user.instance.size()+1, const_cast<char*>(user.instance.c_str()), user.port.size()+1, const_cast<char*>(user.port.c_str()), *cid, m_rid);
			}
			delete cid;
		}
	} else {
		m_called_connects.push_back(CalledConnect(user, provider));
	}
}

void
gluon::impl::CharmManager::connect_array ( EndPoint user, EndPoint provider )
{
	if ( m_backbone_access ) {
		++m_rid;
		if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
			m_backbone_access->wid(m_rid);
		}
		// the connector local to 	the provider is responsible for connection
		if ( local_namespace_management->instances().find(provider.instance) != local_namespace_management->instances().end() ) {
			CkArrayID* gid =  static_cast<CkArrayID*>(raw_component_management->get(provider));
			if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
				m_backbone_access->thisProxy.ckLocalBranch()->set_array(user.instance.size()+1, const_cast<char*>(user.instance.c_str()), user.port.size()+1, const_cast<char*>(user.port.c_str()), *gid, m_rid);
			} else {
				m_backbone_access->thisProxy[namespace_management->get_rank(user.instance)].set_array(user.instance.size()+1, const_cast<char*>(user.instance.c_str()), user.port.size()+1, const_cast<char*>(user.port.c_str()), *gid, m_rid);
			}
			delete gid;
		}
	} else {
		m_called_array_connects.push_back(CalledConnect(user, provider));
	}
}

void
gluon::impl::CharmManager::connect_group ( EndPoint user, EndPoint provider )
{
	if ( m_backbone_access ) {
		++m_rid;
		if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
			m_backbone_access->wid(m_rid);
		}
		// the connector local to 	the provider is responsible for connection
		if ( local_namespace_management->instances().find(provider.instance) != local_namespace_management->instances().end() ) {
			CkGroupID* gid =  static_cast<CkGroupID*>(raw_component_management->get(provider));
			if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
				m_backbone_access->thisProxy.ckLocalBranch()->set_group(user.instance.size()+1, const_cast<char*>(user.instance.c_str()), user.port.size()+1, const_cast<char*>(user.port.c_str()), *gid, m_rid);
			} else {
				m_backbone_access->thisProxy[namespace_management->get_rank(user.instance)].set_group(user.instance.size()+1, const_cast<char*>(user.instance.c_str()), user.port.size()+1, const_cast<char*>(user.port.c_str()), *gid, m_rid);
			}
			delete gid;
		}
	} else {
		m_called_group_connects.push_back(CalledConnect(user, provider));
	}
}
