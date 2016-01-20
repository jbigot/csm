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
#include <iostream>
#include <list>
#include <string>

#include <llcmcpp3.h>
#include <mpimanagement.h>
#include <namespacemanagement.h>

using ::std::abort;
using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

namespace llcmcpp {

namespace impl {

class ClusterMpiManagementFilter:
		virtual public MpiManagement
{
public:
	NamespaceManagement* cluster_namespace_management;
	
	NamespaceManagement* grid_namespace_management;
	
	MpiManagement* mpi_management;
	
	ClusterMpiManagementFilter ():
			cluster_namespace_management(0),
			grid_namespace_management(0),
			mpi_management(0)
	{}
	
	virtual void mpi_connect ( list< EndPoint > group )
	{
		string instance = group.front().instance;
		if ( cluster_namespace_management->instances().find(instance) != cluster_namespace_management->instances().end() ) {
#ifndef NDEBUG
			cerr << "[ClusterMpiManagementFilter] "<<instance<<" is on this cluster"<<endl;
#endif
			mpi_management->mpi_connect(group);
		} else {
			if ( grid_namespace_management->instances().find(instance) == grid_namespace_management->instances().end() ) {
				cerr << "[ClusterMpiManagementFilter] *** Error: invalid instance: " << instance << endl;
				abort();
			}
		}
	}
	
};

LCMP(ClusterMpiManagementFilter)
	L_CPP_PROVIDE(MpiManagement, mpi_manager);
	L_CPP_USE(NamespaceManagement, cluster_namespace_management);
	L_CPP_USE(NamespaceManagement, grid_namespace_management);
	L_CPP_USE(MpiManagement, mpi_management);
LEND

}

}
