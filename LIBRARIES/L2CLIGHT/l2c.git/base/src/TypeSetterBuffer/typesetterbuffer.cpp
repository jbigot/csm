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
#include <typesetter.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class TypeSetterBuffer:
		virtual public Go,
		virtual public TypeSetter
{
public:
	TypeSetter* typed_configuration;
	
	TypeSetterBuffer ():
			typed_configuration(0)
	{}
	
	virtual void configure ( EndPoint property, string type, string value )
	{
		m_buffer.push_back(TypedConfiguration(property, type, value));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[TypeSetterBuffer] configurating!" << endl;
#endif
		for ( list< TypedConfiguration >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			typed_configuration->configure(it->property, it->type, it->value);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[TypeSetterBuffer] configuration done!" << endl;
#endif
	}
	
private:
	struct TypedConfiguration {
		EndPoint property; string type; string value;
		TypedConfiguration ( EndPoint property, string type, string value ): property(property), type(type), value(value) {}
	};
	
	list< TypedConfiguration > m_buffer;
	
};

LCMP(TypeSetterBuffer)
	L_CPP_PROVIDE(Go, go);
	L_CPP_PROVIDE(TypeSetter, typed_configurer);
	L_CPP_USE(TypeSetter, typed_configuration);
LEND

}

}
