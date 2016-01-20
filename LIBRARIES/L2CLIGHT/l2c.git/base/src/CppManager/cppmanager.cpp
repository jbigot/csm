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
#include <stdexcept>
#include <string>
#include <vector>

#include <cppmanagement.h>
#include <cppmanagement2.h>
#include <go.h>
#include <host.h>
#include <host3.h>
#include <llcmcpp3.h>
#include <rawcomponentmanagement.h>
#include <systemproperty.h>
#include <start.h>

using ::std::cerr;
using ::std::endl;
using ::std::make_pair;
using ::std::map;
using ::std::out_of_range;
using ::std::string;
using ::std::vector;

namespace llcmcpp {

namespace impl {

class CppManager:
		virtual public CppManagement,
		virtual public CppManagement2,
		virtual public SystemProperty,
		virtual public Start
{
public:
	vector< const char* >* args;
	
	Host* host;

	Host3* host3;
	
	RawComponentManagement* raw_component_management;
	
	CppManager ():
			args(0),
			host(0),
			host3(0),
			raw_component_management(0)
	{
	}
	
	virtual void connect ( EndPoint user, EndPoint provider )
	{
#ifndef NDEBUG
		cerr << "[CppManager] connection "<<user.instance<<"."<<user.port<<" <-- "<<provider.instance<<"."<<provider.port << endl;
#endif
		void* value = raw_component_management->get(provider);
#ifndef NDEBUG
		if (!value) cerr << "[CppManager] ** Warning: " << provider.instance << "." << provider.port << " undefined" << endl;
#endif
		raw_component_management->set(user, value);
	}
	
	virtual void configure ( EndPoint property, int8_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, uint8_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, int16_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, uint16_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, int32_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, uint32_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, int64_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, uint64_t value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, double value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, std::complex< double > value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void configure ( EndPoint property, std::string value )
	{
		raw_component_management->set(property, &value);
	}
	
	virtual void system_configure( EndPoint property, string value_name )
	{
		if ( value_name == "argc" && args ) {
			size_t argc = args->size();
			raw_component_management->set(property, &argc);
			return;
		}
		if ( value_name == "argv" && args ) {
			const char** argv = args->data();
			raw_component_management->set(property, &argv);
			return;
		}
		if ( value_name == "host" && host ) {
			raw_component_management->set(property, host);
			return;
		}
		if ( value_name == "host3" && host3 ) {
			raw_component_management->set(property, host3);
			return;
		}
		try {
			raw_component_management->set(property, m_system_properties.at(value_name));
		} catch ( const out_of_range& e ) {
			cerr << "[CppManager] ** Error: no such system property: " << value_name << endl;
			throw e;
		}
	}
	
	virtual void start ( EndPoint property )
	{
#ifndef NDEBUG
		cerr << "[CppManager] Invoking go on " << property.instance << "." << property.port << endl;
#endif
		Go* go = static_cast<Go*>(raw_component_management->get(property));
		if (go) go->go();
		else {
			cerr << "[CppManager] ** Error: no such port: " << property.instance << '.' << property.port << endl;
			abort();
		}
	}
	
	virtual void set_property ( string name, void* value )
	{
		if ( !m_system_properties.insert(make_pair(name, value)).second ) {
			cerr << "[CppManager] *** Warning: Property " << name << " already set!" << endl;
		}
	}
	
private:
	map< string, void* > m_system_properties;
	
};

LCMP(CppManager)
	L_CPP_PROVIDE(CppManagement, cpp_manager);
	L_CPP_PROVIDE(CppManagement2, cpp_manager2);
	L_CPP_PROVIDE(SystemProperty, system_properties);
	L_CPP_PROVIDE(Start, start_manager);
	L_CPP_USE(vector< const char* >, args);
	L_CPP_USE(Host, host);
	L_CPP_USE(Host3, host3);
	L_CPP_USE(RawComponentManagement, raw_component_management);
LEND

}

}