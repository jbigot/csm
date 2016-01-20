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

#include <mpi.h>

#include <iostream>
#include <map>
#include <set>
#include <string>

#include <clusterinstantiationmanagement.h>
#include <clusterplacementmanagement.h>
#include <llcmcpp3.h>
#include <localinstantiationmanagement.h>
#include <mpiaccess.h>
#include <mpimanagement.h>
#include <namespacemanagement.h>

using ::std::cerr;
using ::std::endl;
using ::std::map;
using ::std::set;
using ::std::string;

namespace llcmcpp {

namespace impl {

class ClusterNamespaceManager :
		virtual public ClusterInstantiationManagement,
		virtual public ClusterPlacementManagement,
		virtual public NamespaceManagement
{
public:
	HostId* local_rank;
	
	LocalInstantiationManagement* local_instantiation_management;
	
	ClusterNamespaceManager ():
			local_rank(0),
			local_instantiation_management(0)
	{}
	
	virtual void instantiate ( HostId host, string id, string type, string from = "" )
	{
#ifndef NDEBUG
		cerr << "[ClusterNamespaceManager] instanciating if right host" << endl;
#endif
		if ( host == *local_rank ) {
#ifndef NDEBUG
			cerr << "[ClusterNamespaceManager] right host" << endl;
#endif
			local_instantiation_management->instantiate(id, type, from);
		} else {
#ifndef NDEBUG
			cerr << "[ClusterNamespaceManager] wrong host (" << host << " != " << *local_rank << ')' << endl;
#endif
		}
		m_locations[id] = host;
		m_instances.insert(id);
	}
	
	virtual HostId get_rank ( string instance )
	{
#ifndef NDEBUG
		cerr << "[ClusterNamespaceManager] lookup rank of "<<instance<<endl;
#endif
		return m_locations.at(instance);
	}
	
	virtual const set< string >& instances ()
	{
		return m_instances;
	}
	
private:
	//! mapping ComponentId -> HostId (mainly used for mpi connections)
	map< string, HostId > m_locations;
	set< string > m_instances;
	
};

LCMP(ClusterNamespaceManager)
	L_CPP_PROVIDE(ClusterInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(ClusterPlacementManagement, cluster_namespace_manager);
	L_CPP_PROVIDE(NamespaceManagement, namespace_manager);
	L_CPP_USE(HostId, local_rank);
	L_CPP_USE(LocalInstantiationManagement, local_instantiation_management);
LEND

}

}
