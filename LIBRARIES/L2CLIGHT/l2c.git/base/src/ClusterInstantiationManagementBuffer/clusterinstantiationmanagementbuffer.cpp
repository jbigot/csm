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

#include <clusterinstantiationmanagement.h>
#include <go.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

/*! \brief A component that stores the instruction for assembly creation and plays it when required.
 */
class ClusterInstantiationManagementBuffer:
		virtual public ClusterInstantiationManagement,
		virtual public Go
{
public:
	ClusterInstantiationManagement* instantiation_management;
	
	ClusterInstantiationManagementBuffer ():
			instantiation_management(0)
	{}
	
	virtual void instantiate ( HostId host, string id, string type, string from = "" )
	{
		m_buffer.push_back(ClusterInstantiation(host, id, type, from));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[ClusterInstantiationManagementBuffer] instantiating!" << endl;
#endif
		for ( list< ClusterInstantiation >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			instantiation_management->instantiate(it->host, it->name, it->type, it->from);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[ClusterInstantiationManagementBuffer] instantiation done!" << endl;
#endif
	}
	
private:
	struct ClusterInstantiation {
		HostId host; string name; string type; string from;
		ClusterInstantiation ( HostId host, string name, string type, string from = "" ): host(host), name(name), type(type), from(from) {}
	};
	
	list< ClusterInstantiation > m_buffer;
};

LCMP(ClusterInstantiationManagementBuffer)
	L_CPP_PROVIDE(ClusterInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(ClusterInstantiationManagement, instantiation_management);
LEND

}

}

