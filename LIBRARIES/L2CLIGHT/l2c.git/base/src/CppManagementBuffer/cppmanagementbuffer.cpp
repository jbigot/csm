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

#include <complex>
#include <iostream>
#include <list>
#include <string>

#include <cppmanagement2.h>
#include <go.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::complex;
using ::std::endl;
using ::std::list;
using ::std::string;

class CppManagementBuffer:
		virtual public CppManagement2,
		virtual public Go
{
public:
	CppManagement2* cpp_management;
	
	CppManagementBuffer ():
			cpp_management(0)
	{}
	
public:
	virtual void configure ( EndPoint property, int8_t value )
	{
		m_buffer.push_back(new Configuration< int8_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint8_t value )
	{
		m_buffer.push_back(new Configuration< uint8_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, int16_t value )
	{
		m_buffer.push_back(new Configuration< int16_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint16_t value )
	{
		m_buffer.push_back(new Configuration< uint16_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, int32_t value )
	{
		m_buffer.push_back(new Configuration< int32_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint32_t value )
	{
		m_buffer.push_back(new Configuration< uint32_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, int64_t value )
	{
		m_buffer.push_back(new Configuration< int64_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, uint64_t value )
	{
		m_buffer.push_back(new Configuration< uint64_t >(property, value));
	}
	
	virtual void configure ( EndPoint property, double value )
	{
		m_buffer.push_back(new Configuration< double >(property, value));
	}
	
	virtual void configure ( EndPoint property, complex< double > value )
	{
		m_buffer.push_back(new Configuration< complex< double > >(property, value));
	}
	
	virtual void configure ( EndPoint property, string value )
	{
		m_buffer.push_back(new Configuration< string >(property, value));
	}
	
	virtual void system_configure(EndPoint property, string value_name)
	{
		m_buffer.push_back(new SystemConfiguration(property, value_name));
	}
	
	virtual void connect ( EndPoint user, EndPoint provider )
	{
		m_buffer.push_back(new Connection(user, provider));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[CppManagementBuffer] configurating!" << endl;
#endif
		for ( list< Action* >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			(*it)->execute(this);
			delete (*it);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[CppManagementBuffer] configuration done!" << endl;
#endif
	}
	
private:
	class Action
	{
	public:
		virtual void execute ( CppManagementBuffer* builder ) = 0;
		virtual ~Action () {}
	};
	
	struct SystemConfiguration : virtual public Action {
		EndPoint property; string value_name;
		SystemConfiguration ( EndPoint property, string value_name ): property(property), value_name(value_name) {}
		virtual void execute ( CppManagementBuffer* builder ) { builder->cpp_management->system_configure(property, value_name); }
	};
	
	template < class T >
	struct Configuration : virtual public Action {
		EndPoint property; T value;
		Configuration ( EndPoint property, T value ): property(property), value(value) {}
		virtual void execute ( CppManagementBuffer* builder ) { builder->cpp_management->configure(property, value); }
	};
	
	struct Connection : virtual public Action {
		EndPoint user; EndPoint provider;
		Connection ( EndPoint user, EndPoint provider ): user(user), provider(provider) {}
		virtual void execute ( CppManagementBuffer* builder ) { builder->cpp_management->connect(user, provider); }
	};
	
	list< Action* > m_buffer;
};

LCMP(CppManagementBuffer)
	L_CPP_PROVIDE(CppManagement2, cpp_manager);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(CppManagement2, cpp_management);
LEND

}

}

