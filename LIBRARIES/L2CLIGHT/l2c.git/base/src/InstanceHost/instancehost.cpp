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
#include <sstream>

#include <dlfcn.h>

#include <llcmcpp3.h>

#include "instancehost.h"

namespace llcmcpp {

using ::std::map;
using ::std::string;

InstanceHost::InstanceHost():
		m_available_compid(0),
		print(0),
		m_host_legacy(*this)
{
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] Created");
#endif
}

InstanceHost::~InstanceHost()
{
	for ( std::map< ComponentInstance, Component* >::const_iterator it = m_components.begin(); it != m_components.end(); ++ it ) {
		delete it->second;
	}
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] Destroyed");
#endif
}

typedef void (*Registration)();

ComponentFactory 
InstanceHost::factory ( string name, string from )
{
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] getting factory for component " << name);
#endif
	
	map<string, ComponentFactory>::iterator it = m_factories.find(name);
	if ( it != m_factories.end() ) {
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] factory found in cache");
#endif
		return it->second;
	}
	
	if ( from == "" ) from = string("lib")+name+".so";
	void* handle = this->handle(from);
	
	// Creation
	string fname = string("_llcmcpp_create_")+name;
	dlerror(); /* Clear any existing error */
	ComponentFactory factory = (ComponentFactory) dlsym(handle, fname.c_str());
	char* error = dlerror();
	if ( error )  {
PRINTLNERR(print, "[InstanceHost] *** Error:"<<error);
	  exit(EXIT_FAILURE);
	}
	m_factories[name] = factory;
	
	// registration
	fname = string("_llcmcpp_register_")+name;
	dlerror(); /* Clear any existing error */
	Registration reg = (Registration) dlsym(handle, fname.c_str());
	error = dlerror();
	if ( !error ) {
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] Registering component type "<<name)
#endif
		reg();
	}
#ifndef NDEBUG
	else  {
PRINTLNERR(print, "[InstanceHost-"<<this<<"] no registration for component type "<<name);
PRINTLNERR(print, "[InstanceHost-"<<this<<"] Because:"<<error);
	}
#endif
	
	
	return factory;
}

void*
InstanceHost::get(ComponentInstance cid, string psrv)
{
	Component *pc = m_components[cid];
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] getting value " << psrv << " from component #" << cid);
#endif   
	return pc->get(psrv.c_str());
}

void* 
InstanceHost::handle ( string library )
{
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] loading DLL " << library);
#endif
	
	map<string, void*>::iterator it = m_handles.find(library);
	if ( it != m_handles.end() ) {
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] DLL found in cache");
#endif
		return it->second;
	}
	
	dlerror();
	void* handle = dlopen(library.c_str(), RTLD_NOW|RTLD_LOCAL); // | RTLD_DEEPBIND => NO !!
	char* error = dlerror();
	if ( !handle ) {
PRINTLNERR(print, error);
	  exit(EXIT_FAILURE);
	}
	m_handles[library] = handle;
	return handle;  
}

Host3::ComponentInstance
InstanceHost::instantiate ( string type, string from )
{
	ComponentFactory factory = this->factory(type, from);
	ComponentInstance id = m_available_compid++;
	
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] instantiating component #" << id << ": " << type);
#endif
	m_components[id] = factory();
	
	return id;
}

void
InstanceHost::preload ( string type, string from )
{
	this->factory(type, from);
}

void
InstanceHost::destroy( ComponentInstance instance )
{
	map< ComponentInstance, Component* >::iterator it = m_components.find(instance);
	if ( it == m_components.end() ) {
PRINTLNERR(print, "[InstanceHost-"<<this<<"] *** Error: deleting non existing component!");
		abort();
	}
	delete it->second;
	m_components.erase(it);
}


void
InstanceHost::set ( ComponentInstance cid, string pclt, void* value )
{
#ifndef NDEBUG
PRINTLNERR(print, "[InstanceHost-"<<this<<"] setting value " << pclt << " on component #" << cid);
#endif
	m_components.at(cid)->set(pclt.c_str(), value);
}

LCMP(InstanceHost)
	L_CPP_PROVIDE2(Host, host)
	L_CPP_PROVIDE2(Host2, host2)
	L_CPP_PROVIDE(Host3, host3)
	L_CPP_USE(Print, print)
LEND

}
