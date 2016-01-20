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
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <utility>

#include <corbaaccess.h>
#include <clusterinstantiationmanagement.h>
#include <clusterplacementmanagement.h>
#include <distributedplacementmanagement.h>
#include <gridinstantiationmanagement.h>
#include <gridplacementmanagement.h>
#include <initializer.h>
#include <llcmcpp3.h>
#include <mpiaccess.h>
#include <namespacemanagement.h>

using ::std::cerr;
using ::std::endl;
using ::std::map;
using ::std::pair;
using ::std::set;
using ::std::string;
using ::std::ostringstream;

namespace llcmcpp {

namespace impl {

class GridNamespaceManager:
		virtual public ClusterPlacementManagement,
		virtual public DistributedPlacementManagement,
		virtual public GridInstantiationManagement,
		virtual public GridPlacementManagement,
		virtual public Initializer,
		virtual public NamespaceManagement
{
public:
	string local_host;
	
	string local_cluster;
	
	HostId* local_rank;
	
	ClusterInstantiationManagement* cluster_instantiation_management;
	
	GridNamespaceManager ():
			local_rank(0),
			cluster_instantiation_management(0)
	{}
	
	virtual string get_cluster ( string instance )
	{
		return m_placement.at(instance).first;
	}
	
	virtual string get_host ( string instance )
	{
		std::ostringstream out;
		pair< string, HostId > p = m_placement.at(instance);
		out << p.first << p.second;
		return out.str();
	}
	
    virtual HostId get_rank ( string instance )
	{
		return m_placement.at(instance).second;
	}
	
	virtual const set< string >& instances ()
	{
		return m_instances;
	}
	
    virtual void initialize ( int* argc, char*** argv )
	{
		ostringstream dat;
		dat << local_cluster << *local_rank;
		local_host = dat.str();
#ifndef NDEBUG
		cerr << "[GridNamespaceManager] computed my host as " << local_host << endl;
#endif
	}
	
	virtual void instantiate ( string cluster, HostId rank, string id, string type, string from = "" )
	{
#ifndef NDEBUG
		cerr << "[GridNamespaceManager] instanciating if right cluster" << endl;
#endif
		if ( cluster == local_cluster ) {
#ifndef NDEBUG
			cerr << "[GridNamespaceManager] right cluster" << endl;
#endif
			cluster_instantiation_management->instantiate(rank, id, type, from);
		} else {
#ifndef NDEBUG
			cerr << "[GridNamespaceManager] wrong cluster (" << cluster << ", " << rank << ") != (" << local_cluster << ", " << *local_rank << ')' << endl;
#endif
		}
		m_placement[id] = make_pair(cluster, rank);
		m_instances.insert(id);
	}
	
private:
	map< string, pair< string, HostId > > m_placement;
	
	set< string > m_instances;
	
};

LCMP(GridNamespaceManager)
	L_PROPERTY(string, local_cluster);
	L_CPP_PROVIDE(ClusterPlacementManagement, cluster_placement_manager);
	L_CPP_PROVIDE(DistributedPlacementManagement, distributed_placement_manager);
	L_CPP_PROVIDE(GridPlacementManagement, grid_placement_manager);
	L_CPP_PROVIDE(GridInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(Initializer, initializer);
	L_CPP_PROVIDE2(string, local_cluster);
	L_CPP_PROVIDE2(string, local_host);
	L_CPP_PROVIDE(NamespaceManagement, namespace_manager);
	L_CPP_USE(ClusterInstantiationManagement, cluster_instantiation_management);
	L_CPP_USE(HostId, local_rank);
LEND

}

}
