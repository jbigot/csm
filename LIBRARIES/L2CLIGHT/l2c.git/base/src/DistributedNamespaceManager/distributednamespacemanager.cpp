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

#include <distributedinstantiationmanagement.h>
#include <distributedplacementmanagement.h>
#include <limits.h>
#include <llcmcpp3.h>
#include <localinstantiationmanagement.h>
#include <namespacemanagement.h>
#include <unistd.h>

#ifdef HOST_NAME_MAX
#define L2C_DNM_HOST_NAME_MAX (HOST_NAME_MAX+1)
#else
#ifdef _POSIX_HOST_NAME_MAX
#define L2C_DNM_HOST_NAME_MAX (_POSIX_HOST_NAME_MAX+1)
#else
#define L2C_DNM_HOST_NAME_MAX (256+1)
#endif
#endif

using ::std::cerr;
using ::std::endl;
using ::std::map;
using ::std::set;
using ::std::string;

namespace llcmcpp {

namespace impl {

class DistributedNamespaceManager:
		virtual public DistributedInstantiationManagement,
		virtual public DistributedPlacementManagement,
		virtual public NamespaceManagement
{
public:
	LocalInstantiationManagement* local_instantiation_management;
	
	string local_host;
	
	DistributedNamespaceManager ():
			local_instantiation_management(0)
	{
		char tmp_name[L2C_DNM_HOST_NAME_MAX];
		if ( ! gethostname(tmp_name, L2C_DNM_HOST_NAME_MAX) ) {
			tmp_name[L2C_DNM_HOST_NAME_MAX-1]=0;
			local_host = tmp_name;
		}
	}
	
	virtual string get_host ( string instance )
	{
		return m_placement.at(instance);
	}
	
	virtual const set< string >& instances ()
	{
		return m_instances;
	}
	
	virtual void instantiate ( string host, string id, string type, string from = "" )
	{
#ifndef NDEBUG
		cerr << "[DistributedNamespaceManager] instanciating if right host" << endl;
#endif
		if ( host == local_host ) {
#ifndef NDEBUG
			cerr << "[DistributedNamespaceManager] right host" << endl;
#endif
			local_instantiation_management->instantiate(id, type, from);
		} else {
#ifndef NDEBUG
			cerr << "[DistributedNamespaceManager] wrong host (" << host << " != " << local_host << ')' << endl;
#endif
		}
		m_placement[id] = host;
		m_instances.insert(id);
	}
	
private:
	map< string, string > m_placement;
	
	set< string > m_instances;
	
};

LCMP(DistributedNamespaceManager)
	L_PROPERTY(string, local_host);
	L_CPP_PROVIDE(DistributedInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE2(string, local_host);
	L_CPP_PROVIDE(DistributedPlacementManagement, distributed_namespace_manager);
	L_CPP_PROVIDE(NamespaceManagement, namespace_manager);
	L_CPP_USE(LocalInstantiationManagement, local_instantiation_management);
LEND

}

}
