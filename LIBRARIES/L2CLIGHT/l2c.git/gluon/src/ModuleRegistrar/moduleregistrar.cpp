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

#include <charm.h>
#include <dlfcn.h>
#include <llcmcpp/clusterinstantiationmanagement.h>
#include <llcmcpp/host2.h>
#include <llcmcpp/llcmcpp3.h>

#include <charminstantiationmanagement.h>

namespace gluon {

namespace impl {

using ::llcmcpp::ClusterInstantiationManagement;
using ::llcmcpp::Host2;
using ::llcmcpp::HostId;
using ::std::string;

class ModuleRegistrar:
		virtual public CharmInstantiationManagement,
		virtual public ClusterInstantiationManagement
{
public:
	CharmInstantiationManagement* charm_instantiation_management;
	
	Host2* host;
	
	ClusterInstantiationManagement* instantiation_management;
	
	ModuleRegistrar ():
			charm_instantiation_management(0),
			host(0),
			instantiation_management(0)
	{
	}
	
	virtual void instantiate ( HostId hostid, string id, string type, string from )
	{
#ifndef NDEBUG
	  CkPrintf("[ModuleRegistrat] instantiate %s\n", type.c_str());
#endif
		host->preload(type, from);
		instantiation_management->instantiate(hostid, id, type, from);
	}
	
	virtual void instantiate_array ( string id, uint32_t size, string type, string from )
	{
		host->preload(type, from);
		charm_instantiation_management->instantiate_array(id, size, type, from);
	}
	
	virtual void instantiate_bound_array ( string id, uint32_t size, string bound_to, string type, string from )
	{
		host->preload(type, from);
		charm_instantiation_management->instantiate_bound_array(id, size, bound_to, type, from);
	}
	
	virtual void instantiate_group ( string id, string type, string from )
	{
		host->preload(type, from);
		charm_instantiation_management->instantiate_group(id, type, from);
	}
	
};

LCMP(ModuleRegistrar)
	L_CPP_PROVIDE(CharmInstantiationManagement, charm_instantiation_manager);
	L_CPP_PROVIDE(ClusterInstantiationManagement, instantiation_manager);
	L_CPP_USE(CharmInstantiationManagement, charm_instantiation_management);
	L_CPP_USE(Host2, host);
	L_CPP_USE(ClusterInstantiationManagement, instantiation_management);
LEND

}

}
