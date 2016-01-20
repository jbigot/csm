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

#include <string>

#include <clusterinstantiationmanagement.h>
#include <distributedinstantiationmanagement.h>
#include <gridinstantiationmanagement.h>
#include <llcmcpp3.h>
#include <localinstantiationmanagement.h>

using ::std::string;

namespace llcmcpp {

namespace impl {

class LocalInstantiationFilter:
		virtual public DistributedInstantiationManagement,
		virtual public GridInstantiationManagement
{
public:
	LocalInstantiationManagement* instantiation_management;
	
	ClusterInstantiationManagement* cluster_instantiation_management;
	
	string local_name;
	
	LocalInstantiationFilter():
			instantiation_management(0),
			cluster_instantiation_management(0),
			local_name("")
	{}
	
	virtual void instantiate(string host, string id, string type, string from = "")
	{
		if ( host == local_name ) {
			instantiation_management->instantiate(id, type, from);
		}
	}
	
	virtual void instantiate(string cluster, HostId host, string id, string type, string from = "")
	{
		if ( cluster == local_name ) {
			cluster_instantiation_management->instantiate(host, id, type, from);
		}
	}
	
};

LCMP(LocalInstantiationFilter)
	L_PROPERTY(string, local_name);
	L_CPP_PROVIDE(DistributedInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(GridInstantiationManagement, grid_instantiation_manager);
	L_CPP_USE(LocalInstantiationManagement, instantiation_management);
	L_CPP_USE(ClusterInstantiationManagement, cluster_instantiation_management);
LEND

}

}
