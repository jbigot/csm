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

#ifndef LLCMCPP_LLCMCOMMON_H
#define LLCMCPP_LLCMCOMMON_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <string>

#include "component.h"

namespace llcmcpp {

namespace impl {

template < class Cmp >
struct Setter
{
	virtual void operator() ( Cmp*, void* ) = 0;
	
};

template < class Cmp >
struct Getter
{
	virtual void* operator() ( Cmp* ) = 0;
	
};

template < class Cmp >
class BasicComponent:
		public Component
{
public:
	Cmp instance;
	
	const char* name;
	
	BasicComponent (): name(0) {}
	
	BasicComponent ( const char* name ): name(name) {}
	
	BasicComponent ( const char* name, const Cmp& inst  ): name(name), instance(inst) {}
	
	std::map< std::string, Setter< Cmp >* > input_properties;
	
	std::map< std::string, Getter< Cmp >* > output_properties;
	
	void* get(const char* propname)
	{
		typename std::map< std::string, Getter< Cmp >* >::iterator it = output_properties.find(std::string(propname));
		if ( it == output_properties.end() ) {
			if ( std::string("lifecycle") != propname ) {
				std::cerr << '[' << name << "] *** Error: output property \""<<propname<<"\" does not exist!" << std::endl;
				std::abort();
			}
			return 0;
		}
		return (*(it->second))(&instance);
	}
	
	virtual void set(const char* propname, void* value)
	{
		typename std::map< std::string, Setter< Cmp >* >::iterator it = input_properties.find(std::string(propname));
		if ( it == input_properties.end() ) {
			std::cerr << '[' << name << "] *** Error: input property \""<<propname<<"\" does not exist!" << std::endl;
			std::abort();
		}
		(*(it->second))(&instance, value);
	}
	
};

}

}

#define LCMP(type) \
extern "C" ::llcmcpp::Component* _llcmcpp_create_##type () { \
	::llcmcpp::impl::BasicComponent< type >* cmp = new ::llcmcpp::impl::BasicComponent< type >; \
	cmp->name = #type ;\
	typedef type Cmp;

#define LEND return cmp; }

#endif // LLCMCPP_LLCMCOMMON_H
