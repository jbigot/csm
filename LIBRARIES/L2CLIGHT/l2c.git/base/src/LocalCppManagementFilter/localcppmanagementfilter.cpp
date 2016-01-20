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
#include <string>

#include <cppmanagement.h>
#include <cppmanagement2.h>
#include <llcmcpp3.h>
#include <namespacemanagement.h>
#include <start.h>

using ::std::abort;
using ::std::cerr;
using ::std::endl;
using ::std::string;

namespace llcmcpp {

namespace impl {

class LocalCppManagementFilter:
		virtual public CppManagement,
		virtual public CppManagement2,
		virtual public Start
{
public:
	NamespaceManagement* global_namespace_management;
	
	CppManagement* local_cpp_management;
	
	NamespaceManagement* local_namespace_management;
	
	LocalCppManagementFilter ():
			global_namespace_management(0),
			local_cpp_management(0),
			local_namespace_management(0)
	{
	}
	
	virtual void configure ( EndPoint property, string value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, int8_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, uint8_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, int16_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, uint16_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, int32_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, uint32_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, int64_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, uint64_t value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, double value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void configure ( EndPoint property, std::complex< double > value)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->configure(property, value);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void connect(EndPoint user, EndPoint provider)
	{
		if ( local_namespace_management->instances().find(user.instance) != local_namespace_management->instances().end() ) {
			if ( local_namespace_management->instances().find(provider.instance) == local_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: while connecting "<<user.instance<<'.'<<user.port<<" <-- "<<provider.instance<<'.'<<provider.port<<endl;
				cerr << "[LocalCppManagementFilter] *** Error: invalid connection: user is local, but provider is not!" << endl;
				abort();
			}
			local_cpp_management->connect(user, provider);
		} else {
			if ( local_namespace_management->instances().find(provider.instance) != local_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: while connecting "<<user.instance<<'.'<<user.port<<" <-- "<<provider.instance<<'.'<<provider.port<<endl;
				cerr << "[LocalCppManagementFilter] *** Error: invalid connection: provider is local, but user is not!" << endl;
				abort();
			}
			if ( global_namespace_management->instances().find(user.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: while connecting "<<user.instance<<'.'<<user.port<<" <-- "<<provider.instance<<'.'<<provider.port<<endl;
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << user.instance << endl;
				abort();
			}
			if ( global_namespace_management->instances().find(provider.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: while connecting "<<user.instance<<'.'<<user.port<<" <-- "<<provider.instance<<'.'<<provider.port<<endl;
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << provider.instance << endl;
				abort();
			}
		}
	}
	
	virtual void start(EndPoint property)
	{
#ifndef NDEBUG
			cerr << "[LocalCppManagementFilter] start if right host (instance=" << property.instance << ", port=" << property.port << ")" << endl;
#endif // NDEBUG
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
#ifndef NDEBUG
			cerr << "[LocalCppManagementFilter] start(instance=" << property.instance << ", port=" << property.port << ")" << endl;
#endif // NDEBUG
			local_cpp_management->start(property);
		} else {
#ifndef NDEBUG
			cerr << "[LocalCppManagementFilter] wrong host" << endl;
#endif // NDEBUG
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
	virtual void system_configure(EndPoint property, std::string value_name)
	{
		if ( local_namespace_management->instances().find(property.instance) != local_namespace_management->instances().end() ) {
			local_cpp_management->system_configure(property, value_name);
		} else {
			if ( global_namespace_management->instances().find(property.instance) == global_namespace_management->instances().end() ) {
				cerr << "[LocalCppManagementFilter] *** Error: invalid instance: " << property.instance << endl;
				abort();
			}
		}
	}
	
};

LCMP(LocalCppManagementFilter)
	L_CPP_PROVIDE(CppManagement, cpp_manager);
	L_CPP_PROVIDE(CppManagement2, cpp_manager2);
	L_CPP_PROVIDE(Start, start_manager);
	L_CPP_USE(NamespaceManagement, global_namespace_management);
	L_CPP_USE(CppManagement, local_cpp_management);
	L_CPP_USE(NamespaceManagement, local_namespace_management);
LEND

}

}
