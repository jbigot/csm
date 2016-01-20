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
#include <list>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include <clusterplacementmanagement.h>
#include <initializer.h>
#include <llcmcpp3.h>
#include <llcmmpi.h>
#include <mpiaccess.h>
#include <mpimanagement.h>
#include <namespacemanagement.h>
#include <rawcomponentmanagement.h>

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::out_of_range;
using ::std::string;
using ::std::set;
using ::std::vector;

namespace llcmcpp {

namespace impl {

class MpiManager:
		virtual public Initializer,
		virtual public MpiManagement
{
public:
	ClusterPlacementManagement* cluster_namespace_management;
	
	MPI_Comm comm;
	
	HostId local_rank;
	
	RawComponentManagement* raw_component_management;
	
	MpiManager ():
			cluster_namespace_management(0),
			comm(MPI_COMM_NULL),
			raw_component_management(0)
	{}
	
	virtual void initialize(int* argc, char*** argv)
	{
		// this should be done with LifeCycle in "activate"
		// in the bootstrap situation however, MPI is not available at that step
		// a clean solution involves the addition of dedicated MPI & CORBA lifecycle interfaces
		int rank;
		MPI_Comm_rank(comm, &rank);
		local_rank = rank;
	}
	
	virtual void mpi_connect ( list< EndPoint > group)
	{
#ifndef NDEBUG
		cerr << "[MpiManager] mpi_connect: " ;
		for (  list<EndPoint>::iterator it = group.begin(); it != group.end(); ++it ) {
			cerr << it->instance << "." << it->port << "; ";
		}
		cerr << endl;
#endif
		list< EndPoint > local_endpoints;
		vector< int > ranks;
		ranks.reserve(group.size());
		set< HostId > ranks_set;
		for ( list< EndPoint >::const_iterator it = group.begin(); it != group.end(); ++it ) {
			HostId res;
			try {
				res = cluster_namespace_management->get_rank(it->instance);
			} catch ( out_of_range& e ) {
				cerr << "[MpiManager] *** Error: "<<it->instance<<" does not exist!"<<endl;
				abort();
			}
			if ( res == local_rank ) {
				local_endpoints.push_back(*it);
#ifndef NDEBUG
				cerr << "[MpiManager] " << it->instance << " is on this host" << endl;
#endif
			}
			if ( ranks_set.insert(res).second ) ranks.push_back(res);
		}
#ifndef NDEBUG
		{
			cerr << "[MpiManager] communicator {" ;
			vector< int >::iterator it = ranks.begin();
			cerr << *it;
			++it;
			for ( ; it != ranks.end(); ++it ) cerr << ", " << *it;
			cerr << "}" << endl;
		}
#endif
		MPI_Group old_group;
		MPI_Comm_group(comm, &old_group);
		
		MPI_Group new_group;
		MPI_Group_incl(old_group, ranks.size(), ranks.data(), &new_group);
		
		MPI_Comm created_comm;
		MPI_Comm_create(comm, new_group, &created_comm);
		MPI_Group_free(&new_group);
		
		for ( list< EndPoint >::iterator it=local_endpoints.begin(); it != local_endpoints.end(); ++it ) {
			MPI_Comm transferred_comm;
			MPI_Comm_dup(created_comm, &transferred_comm);
			raw_component_management->set(*it, &transferred_comm); // communicators are then copied to the component that's responsible for freeing it.
		}
		if ( ! local_endpoints.empty() ) MPI_Comm_free(&created_comm); // this one is not used anymore ... it existed only if a local user existed
	}
	
};

LCMP(MpiManager)
	L_CPP_PROVIDE(Initializer, initializer);
	L_CPP_PROVIDE2(HostId, local_rank);
	L_CPP_PROVIDE(MpiManagement, mpi_manager);
	L_CPP_USE(ClusterPlacementManagement, cluster_namespace_management);
	L_CPP_USE(RawComponentManagement, raw_component_management);
	L_MPI_COMM(comm);
LEND

}

}

