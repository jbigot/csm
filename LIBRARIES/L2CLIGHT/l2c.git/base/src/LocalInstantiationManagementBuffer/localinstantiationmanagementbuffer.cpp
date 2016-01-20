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
#include <llcmcpp3.h>
#include <localinstantiationmanagement.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class LocalInstantiationManagementBuffer:
		virtual public Go,
		virtual public LocalInstantiationManagement
{
public:
	
	LocalInstantiationManagement* instantiation_management;
	
	LocalInstantiationManagementBuffer ():
			instantiation_management(0)
	{}
	
private:
	struct LocalInstantiation {
		string name; string type; string from;
		LocalInstantiation ( string name, string type, string from = "" ): name(name), type(type), from(from) {}
		virtual void execute ( LocalInstantiationManagementBuffer* builder ) { builder->instantiation_management->instantiate(name, type, from); }
	};
	
	list< LocalInstantiation > m_buffer;
	
public:
	virtual void instantiate ( string name, string type, string from = "" )
	{
		m_buffer.push_back(LocalInstantiation(name, type, from));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[LocalInstantiationManagementBuffer] instantiating!" << endl;
#endif
		for ( list< LocalInstantiation >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			instantiation_management->instantiate(it->name, it->type, it->from);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[LocalInstantiationManagementBuffer] instantiation done!" << endl;
#endif
	}
};

LCMP(LocalInstantiationManagementBuffer)
	L_CPP_PROVIDE(Go, go);
	L_CPP_PROVIDE(LocalInstantiationManagement, instantiation_manager);
	L_CPP_USE(LocalInstantiationManagement, instantiation_management);
LEND

}

}

