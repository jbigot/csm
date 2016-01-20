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

#include <distributedinstantiationmanagement.h>
#include <go.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class DistributedInstantiationManagementBuffer:
		virtual public DistributedInstantiationManagement,
		virtual public Go
{
public:
	DistributedInstantiationManagement* instantiation_management;
	
	DistributedInstantiationManagementBuffer ():
			instantiation_management(0)
	{}
	
private:
	struct Instantiation {
		string host; string name; string type; string from;
		Instantiation ( string host, string name, string type, string from = "" ): host(host), name(name), type(type), from(from) {}
	};
	
	list< Instantiation > m_instantiations;
	
public:
	virtual void instantiate ( string host, string id, string type, string from = "" )
	{
		m_instantiations.push_back(Instantiation(host, id, type, from));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[DistributedInstantiationManagementBuffer] instantiating!" << endl;
#endif
		for ( list< Instantiation >::iterator it = m_instantiations.begin(); it != m_instantiations.end(); ++it ) {
			instantiation_management->instantiate(it->host, it->name, it->type, it->from);
		}
		m_instantiations.clear();
#ifndef NDEBUG
		cerr << "[DistributedInstantiationManagementBuffer] instantiation done!" << endl;
#endif
	}
	
};

LCMP(DistributedInstantiationManagementBuffer)
	L_CPP_PROVIDE(DistributedInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(DistributedInstantiationManagement, instantiation_management);
LEND

}

}

