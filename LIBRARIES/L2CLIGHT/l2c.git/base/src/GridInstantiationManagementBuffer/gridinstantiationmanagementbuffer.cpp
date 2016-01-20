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

#include <go.h>
#include <gridinstantiationmanagement.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class GridInstantiationManagementBuffer:
		virtual public Go,
		virtual public GridInstantiationManagement
{
public:
	GridInstantiationManagement* instantiation_management;
	
	GridInstantiationManagementBuffer ():
			instantiation_management(0)
	{}
	
private:
	struct Instantiation {
		string cluster; HostId host; string name; string type; string from;
		Instantiation ( string cluster, HostId host, string name, string type, string from = "" ): cluster(cluster), host(host), name(name), type(type), from(from) {}
	};
	
	list< Instantiation > m_buffer;
	
public:
	virtual void instantiate ( string cluster, HostId host, string id, string type, string from = "" )
	{
		m_buffer.push_back(Instantiation(cluster, host, id, type, from));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[GridInstantiationManagementBuffer] instantiating!" << endl;
#endif
		for ( list< Instantiation >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			instantiation_management->instantiate(it->cluster, it->host, it->name, it->type, it->from);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[GridInstantiationManagementBuffer] instantiation done!" << endl;
#endif
	}
};

LCMP(GridInstantiationManagementBuffer)
	L_CPP_PROVIDE(Go, go);
	L_CPP_PROVIDE(GridInstantiationManagement, instantiation_manager);
	L_CPP_USE(GridInstantiationManagement, instantiation_management);
LEND

}

}

