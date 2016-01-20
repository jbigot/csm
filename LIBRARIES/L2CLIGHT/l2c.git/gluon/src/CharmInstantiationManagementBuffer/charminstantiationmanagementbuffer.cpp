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

#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp3.h>

#include <charminstantiationmanagement.h>

namespace gluon {

namespace impl {

using ::llcmcpp::Go;
using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class CharmInstantiationManagementBuffer:
		virtual public Go,
		virtual public CharmInstantiationManagement
{
public:
	CharmInstantiationManagement* instantiation_management;
	
	CharmInstantiationManagementBuffer ():
			instantiation_management(0)
	{}
	
private:
	struct Instantiation {
		string name; string type; string from;
		Instantiation ( string name, string type, string from = "" ): name(name), type(type), from(from) {}
	};
	
	struct ArrayInstantiation: public Instantiation {
		uint32_t size;
		ArrayInstantiation ( string name, uint32_t size, string type, string from = "" ): Instantiation(name, type, from), size(size) {}
	};
	
	list< ArrayInstantiation > m_array_buffer;
	
	struct BoundArrayInstantiation: public ArrayInstantiation {
		string bound_to;
		BoundArrayInstantiation ( string name, uint32_t size, string bound_to, string type, string from = "" ): ArrayInstantiation(name, size, type, from), bound_to(bound_to) {}
	};
	
	list< BoundArrayInstantiation > m_bound_array_buffer;
	
	list< Instantiation > m_group_buffer;
	
public:
	virtual void instantiate_array ( string name, uint32_t size, string type, string from = "" )
	{
		m_array_buffer.push_back(ArrayInstantiation(name, size, type, from));
	}
	
	virtual void instantiate_bound_array ( string name, uint32_t size, std::string bound_to, string type, string from = "" )
	{
		m_bound_array_buffer.push_back(BoundArrayInstantiation(name, size, bound_to, type, from));
	}
	
	virtual void instantiate_group ( string name, string type, string from = "" )
	{
		m_group_buffer.push_back(Instantiation(name, type, from));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[CharmInstantiationManagementBuffer] instantiating!" << endl;
#endif
		for ( list< ArrayInstantiation >::iterator it = m_array_buffer.begin(); it != m_array_buffer.end(); ++it ) {
			instantiation_management->instantiate_array(it->name, it->size, it->type, it->from);
		}
		m_array_buffer.clear();
		for ( list< BoundArrayInstantiation >::iterator it = m_bound_array_buffer.begin(); it != m_bound_array_buffer.end(); ++it ) {
			instantiation_management->instantiate_bound_array(it->name, it->size, it->bound_to, it->type, it->from);
		}
		m_bound_array_buffer.clear();
		for ( list< Instantiation >::iterator it = m_group_buffer.begin(); it != m_group_buffer.end(); ++it ) {
			instantiation_management->instantiate_group(it->name, it->type, it->from);
		}
		m_group_buffer.clear();
#ifndef NDEBUG
		cerr << "[CharmInstantiationManagementBuffer] instantiation done!" << endl;
#endif
	}
};

LCMP(CharmInstantiationManagementBuffer)
	L_CPP_PROVIDE(Go, go);
	L_CPP_PROVIDE(CharmInstantiationManagement, instantiation_manager);
	L_CPP_USE(CharmInstantiationManagement, instantiation_management);
LEND

}

}

