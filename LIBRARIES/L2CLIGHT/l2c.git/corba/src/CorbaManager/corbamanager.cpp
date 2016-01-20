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
#include <map>
#include <set>

#include <corbaaccess.h>
#include <corbamanagement.h>
#include <distributedplacementmanagement.h>
#include <llcmcpp3.h>
#include <namespacemanagement.h>
#include <rawcomponentmanagement.h>
#include <rmimanagement.h>

using ::std::abort;
using ::std::cerr;
using ::std::endl;

namespace llcmcpp {

namespace impl {

class CorbaManager :
		virtual public CorbaManagement,
		virtual public RmiManagement
{
public:
	CorbaAccess* corba_access;
	
	NamespaceManagement* global_namespace_management;
	
	NamespaceManagement* local_namespace_management;
	
	DistributedPlacementManagement* namespace_management;
	
	RawComponentManagement* raw_component_management;
	
	CorbaManager ():
			corba_access(0),
			global_namespace_management(0),
			local_namespace_management(0),
			namespace_management(0),
			raw_component_management(0)
	{}
	
	virtual void corba_connect ( EndPoint user, EndPoint provider )
	{
		connect(user, provider);
	}
	
	virtual void connect ( EndPoint user, EndPoint provider )
	{
		// the connector local to the user is responsible for connection
		if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
			raw_component_management->set(user, corba_access->get(namespace_management->get_host(provider.instance), provider));
		} else {
			if ( global_namespace_management->instances().find(user.instance) == global_namespace_management->instances().end() ) {
				cerr << "[CorbaManager] *** Error: invalid instance: " << user.instance << endl;
				abort();
			}
		}
	}
	
};

LCMP(CorbaManager)
	L_CPP_PROVIDE(CorbaManagement, corba_manager);
	L_CPP_PROVIDE(RmiManagement, corba_manager2);
	L_CPP_USE(CorbaAccess, corba_access);
	L_CPP_USE(NamespaceManagement, global_namespace_management);
	L_CPP_USE(NamespaceManagement, local_namespace_management);
	L_CPP_USE(DistributedPlacementManagement, namespace_management);
	L_CPP_USE(RawComponentManagement, raw_component_management);
LEND

}

}
