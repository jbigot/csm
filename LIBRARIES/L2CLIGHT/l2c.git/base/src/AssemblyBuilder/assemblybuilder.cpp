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

#include <complex>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <set>

#include <tinyxml.h>

#include <assemblybuilding.h>
#include <clusterinstantiationmanagement.h>
#include <collective.h>
#include <corbamanagement.h>
#include <cppmanagement.h>
#include <distributedinstantiationmanagement.h>
#include <endpoint.h>
#include <go.h>
#include <gridinstantiationmanagement.h>
#include <lifecycle.h>
#include <llcmcpp3.h>
#include <localinstantiationmanagement.h>
#include <mpimanagement.h>
#include <rmimanagement.h>
#include <typesetter.h>

using ::std::cerr;
using ::std::complex;
using ::std::endl;
using ::std::list;
using ::std::make_pair;
using ::std::map;
using ::std::multimap;
using ::std::pair;
using ::std::set;
using ::std::string;

namespace llcmcpp {

namespace impl {

/*! \brief A component that stores the instruction for assembly creation and plays it when required.
 */
class AssemblyBuilder:
		virtual public AssemblyBuilding,
		virtual public ClusterInstantiationManagement,
		virtual public CorbaManagement,
		virtual public CppManagement,
		virtual public DistributedInstantiationManagement,
		virtual public Go,
		virtual public GridInstantiationManagement,
		virtual public LocalInstantiationManagement,
		virtual public MpiManagement,
		virtual public TypeSetter
{
public:
	class CharmManager:
			virtual public RmiManagement
	{
	public:
		CharmManager ( AssemblyBuilder& builder ):
				m_builder(builder)
		{
		}
		
		virtual void connect(EndPoint user, EndPoint provider)
		{
			m_builder.charm_connect(user, provider);
		}
		
	private:
		AssemblyBuilder& m_builder;
		
	} charm_manager;
	
	class CorbaManager2:
			virtual public RmiManagement
	{
	public:
		CorbaManager2 ( AssemblyBuilder& builder ):
				m_builder(builder)
		{
		}
		
		virtual void connect(EndPoint user, EndPoint provider)
		{
			m_builder.corba_connect(user, provider);
		}
		
	private:
		AssemblyBuilder& m_builder;
		
	} corba_manager2;
	
	RmiManagement* charm_management;
	
	ClusterInstantiationManagement* cluster_instantiation_management;
	
	Collective* collective_access;
	
	CorbaManagement* corba_management;
	
	RmiManagement* corba_management2;
	
	CppManagement* cpp_management;
	
	DistributedInstantiationManagement* distributed_instantiation_management;
	
	GridInstantiationManagement* grid_instantiation_management;
	
	LifeCycle* lifecycle_management;
	
	LocalInstantiationManagement* local_instantiation_management;
	
	MpiManagement* mpi_management;
	
	TypeSetter* typed_configuration;
	
	AssemblyBuilder ():
			charm_manager(*this),
			corba_manager2(*this),
			cluster_instantiation_management(0),
			collective_access(0),
			corba_management(0),
			corba_management2(0),
			cpp_management(0),
			distributed_instantiation_management(0),
			grid_instantiation_management(0),
			lifecycle_management(0),
			local_instantiation_management(0),
			mpi_management(0),
			typed_configuration(0)
	{}
	
private:
	/*! \brief An Action describes something that must be done at a given phase of assembly creation.
	 */
	class Action
	{
	public:
		virtual void execute ( AssemblyBuilder* builder ) = 0;
		virtual ~Action () {}
	};
	
	struct LocalInstantiation : virtual public Action {
		string name; string type; string from;
		LocalInstantiation ( string name, string type, string from = "" ): name(name), type(type), from(from) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->local_instantiation_management->instantiate(name, type, from); }
	};
	
	struct ClusterInstantiation : virtual public Action {
		HostId host; string name; string type; string from;
		ClusterInstantiation ( HostId host, string name, string type, string from = "" ): host(host), name(name), type(type), from(from) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->cluster_instantiation_management->instantiate(host, name, type, from); }
	};
	
	struct DistributedInstantiation : virtual public Action {
		string host; string name; string type; string from;
		DistributedInstantiation ( string host, string name, string type, string from = "" ): host(host), name(name), type(type), from(from) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->distributed_instantiation_management->instantiate(host, name, type, from); }
	};
	
	struct GridInstantiation : virtual public Action {
		string cluster; HostId host; string name; string type; string from;
		GridInstantiation ( string cluster, HostId host, string name, string type, string from = "" ): cluster(cluster), host(host), name(name), type(type), from(from) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->grid_instantiation_management->instantiate(cluster, host, name, type, from); }
	};
	
	struct TypedConfiguration : virtual public Action {
		EndPoint property; string type; string value;
		TypedConfiguration ( EndPoint property, string type, string value ): property(property), type(type), value(value) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->typed_configuration->configure(property, type, value); }
	};
	
	struct SystemConfiguration : virtual public Action {
		EndPoint property; string value_name;
		SystemConfiguration ( EndPoint property, string value_name ): property(property), value_name(value_name) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->cpp_management->system_configure(property, value_name); }
	};
	
	template < class T >
	struct Configuration : virtual public Action {
		EndPoint property; T value;
		Configuration ( EndPoint property, T value ): property(property), value(value) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->cpp_management->configure(property, value); }
	};
	
	struct LocalConnection : virtual public Action {
		EndPoint user; EndPoint provider;
		LocalConnection ( EndPoint user, EndPoint provider ): user(user), provider(provider) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->cpp_management->connect(user, provider); }
	};
	
	struct CorbaConnection : virtual public Action {
		EndPoint user; EndPoint provider;
		CorbaConnection ( EndPoint user, EndPoint provider ): user(user), provider(provider) {}
		virtual void execute ( AssemblyBuilder* builder ) { if ( builder->corba_management2 ) builder->corba_management2->connect(user, provider); else builder->corba_management->corba_connect(user, provider); }
	};
	
	struct CharmConnection : virtual public Action {
		EndPoint user; EndPoint provider;
		CharmConnection ( EndPoint user, EndPoint provider ): user(user), provider(provider) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->charm_management->connect(user, provider); }
	};
	
	struct MpiConnection : virtual public Action {
		list< EndPoint > peers;
		MpiConnection ( list< EndPoint > peers ): peers(peers) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->mpi_management->mpi_connect(peers); }
	};
	
	struct Start : virtual public Action {
		EndPoint property;
		Start ( EndPoint property ): property(property) {}
		virtual void execute ( AssemblyBuilder* builder ) { builder->cpp_management->start(property); }
	};
	
	list< Action* > m_instantiations;
	
	list< Action* > m_configurations;
	
	list< Action* > m_executions;
	
public:
	virtual void instantiate ( string name, string type, string from = "" )
	{
		m_instantiations.push_back(new LocalInstantiation(name, type, from));
	}
	
	virtual void instantiate ( HostId host, string id, string type, string from = "" )
	{
		m_instantiations.push_back(new ClusterInstantiation(host, id, type, from));
	}
	
	virtual void instantiate ( string host, string id, string type, string from = "" )
	{
		m_instantiations.push_back(new DistributedInstantiation(host, id, type, from));
	}
	
	virtual void instantiate ( string cluster, HostId host, string id, string type, string from = "" )
	{
		m_instantiations.push_back(new GridInstantiation(cluster, host, id, type, from));
	}
	
	virtual void configure ( EndPoint property, string type, string value )
	{
		m_configurations.push_back(new TypedConfiguration(property, type, value));
	}
	
	virtual void configure ( EndPoint property, int8_t value )
	{
		m_configurations.push_back(new Configuration< int8_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint8_t value )
	{
		m_configurations.push_back(new Configuration< uint8_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, int16_t value )
	{
		m_configurations.push_back(new Configuration< int16_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint16_t value )
	{
		m_configurations.push_back(new Configuration< uint16_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, int32_t value )
	{
		m_configurations.push_back(new Configuration< int32_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint32_t value )
	{
		m_configurations.push_back(new Configuration< uint32_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, int64_t value )
	{
		m_configurations.push_back(new Configuration< int64_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint64_t value )
	{
		m_configurations.push_back(new Configuration< uint64_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, double value )
	{
		m_configurations.push_back(new Configuration< double >(property, value));
	}
	
	virtual void configure ( EndPoint property, complex< double > value )
	{
		m_configurations.push_back(new Configuration< complex< double > >(property, value));
	}
	
	virtual void configure ( EndPoint property, string value )
	{
		m_configurations.push_back(new Configuration< string >(property, value));
	}
	
	virtual void system_configure(EndPoint property, string value_name)
	{
		m_configurations.push_back(new SystemConfiguration(property, value_name));
	}
	
	virtual void connect ( EndPoint user, EndPoint provider )
	{
		m_configurations.push_back(new LocalConnection(user, provider));
	}
	
    virtual void mpi_connect( list< EndPoint > group )
    {
		m_configurations.push_back(new MpiConnection(group));
	}
	
    virtual void corba_connect ( EndPoint user, EndPoint provider )
    {
		m_configurations.push_back(new CorbaConnection(user, provider));
	}
	
    virtual void charm_connect ( EndPoint user, EndPoint provider )
    {
		m_configurations.push_back(new CharmConnection(user, provider));
	}
	
	virtual void start ( EndPoint property )
	{
		m_executions.push_back(new Start(property));
	}
	
	virtual void instantiation ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] instantiating!" << endl;
#endif
		for ( list< Action* >::iterator it = m_instantiations.begin(); it != m_instantiations.end(); ++it ) {
			(*it)->execute(this);
			delete (*it);
		}
		m_instantiations.clear();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] instantiation done!" << endl;
#endif
	}
	
	virtual void configuration ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] configurating!" << endl;
#endif
		for ( list< Action* >::iterator it = m_configurations.begin(); it != m_configurations.end(); ++it ) {
			(*it)->execute(this);
			delete (*it);
		}
		m_configurations.clear();
		lifecycle_management->configure_complete();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] configuration done!" << endl;
#endif
	}
	
	virtual void activation ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] activating!" << endl;
#endif
		lifecycle_management->activate();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] activation done!" << endl;
#endif
	}
	
	virtual void execution ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] executing!" << endl;
#endif
		for ( list< Action* >::iterator it = m_executions.begin(); it != m_executions.end(); ++it ) {
			(*it)->execute(this);
			delete (*it);
		}
		m_executions.clear();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] executions done!" << endl;
#endif
	}
	
	virtual void deactivation ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] deactivating!" << endl;
#endif
		lifecycle_management->deactivate();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] deactivation done!" << endl;
#endif
	}
	
	virtual void destruction ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] destroying!" << endl;
#endif
		lifecycle_management->destroy();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder] destruction done!" << endl;
#endif
	}
	
	virtual void go ()
	{
		instantiation();
		if ( collective_access ) collective_access->barrier();
		configuration();
		if ( collective_access ) collective_access->barrier();
		activation();
		if ( collective_access ) collective_access->barrier();
		execution();
		if ( collective_access ) collective_access->barrier();
		deactivation();
		if ( collective_access ) collective_access->barrier();
		destruction();
	}
};

LCMP(AssemblyBuilder)
	L_CPP_PROVIDE(AssemblyBuilding, assembly_building);
	L_CPP_PROVIDE2(RmiManagement, charm_manager);
	L_CPP_PROVIDE(ClusterInstantiationManagement, cluster_instantiation_manager);
	L_CPP_PROVIDE(CorbaManagement, corba_manager);
	L_CPP_PROVIDE2(RmiManagement, corba_manager2);
	L_CPP_PROVIDE(CppManagement, cpp_manager);
	L_CPP_PROVIDE(DistributedInstantiationManagement, distributed_instantiation_manager);
	L_CPP_PROVIDE(Go, go);
	L_CPP_PROVIDE(GridInstantiationManagement, grid_instantiation_manager);
	L_CPP_PROVIDE(LocalInstantiationManagement, local_instantiation_manager);
	L_CPP_PROVIDE(MpiManagement, mpi_manager);
	L_CPP_PROVIDE(TypeSetter, typed_configurer);
	L_CPP_USE(RmiManagement, charm_management);
	L_CPP_USE(ClusterInstantiationManagement, cluster_instantiation_management);
	L_CPP_USE(Collective, collective_access);
	L_CPP_USE(CorbaManagement, corba_management);
	L_CPP_USE(RmiManagement, corba_management2);
	L_CPP_USE(CppManagement, cpp_management);
	L_CPP_USE(DistributedInstantiationManagement, distributed_instantiation_management);
	L_CPP_USE(GridInstantiationManagement, grid_instantiation_management);
	L_CPP_USE(LifeCycle, lifecycle_management);
	L_CPP_USE(LocalInstantiationManagement, local_instantiation_management);
	L_CPP_USE(MpiManagement, mpi_management);
	L_CPP_USE(TypeSetter, typed_configuration);
LEND

}

}

