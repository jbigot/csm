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
#include <set>
#include <string>

#include <llcmcpp3.h>
#include <lifecycle.h>
#include <namespacemanagement.h>
#include <rawcomponentmanagement.h>

using ::std::cerr;
using ::std::endl;
using ::std::set;
using ::std::string;

namespace llcmcpp {

namespace impl {

class LifecycleManager:
		virtual public LifeCycle
{
public:
	NamespaceManagement* managed_instances;
	
	RawComponentManagement* raw_component_management;
	
	LifecycleManager ():
			managed_instances(0),
			raw_component_management(0)
	{}
	
	void configure_complete ()
	{
#ifndef NDEBUG
		cerr << "[LifecycleManager] configure_complete" << endl;
#endif
		for ( set< string >::const_iterator it = managed_instances->instances().begin(); it!=managed_instances->instances().end(); ++it ) {
			void* life = raw_component_management->get(EndPoint(*it, "lifecycle"));
			if ( life ) static_cast<LifeCycle*>(life)->configure_complete();
		}
	}
	
	virtual void activate ()
	{
#ifndef NDEBUG
		cerr << "[LifecycleManager] activate" << endl;
#endif
		for ( std::set< std::string >::iterator it = managed_instances->instances().begin(); it!=managed_instances->instances().end(); ++it ) {
			LifeCycle* life = static_cast<LifeCycle*>(raw_component_management->get(EndPoint(*it, "lifecycle")));
			if ( life ) life->activate();
		}
	}
	
	virtual void deactivate ()
	{
#ifndef NDEBUG
		cerr << "[LifecycleManager] deactivate" << endl;
#endif
		for ( std::set< std::string >::iterator it = managed_instances->instances().begin(); it!=managed_instances->instances().end(); ++it ) {
			LifeCycle* life = static_cast<LifeCycle*>(raw_component_management->get(EndPoint(*it, "lifecycle")));
			if ( life ) life->deactivate();
		}
	}
	
	virtual void destroy ()
	{
#ifndef NDEBUG
		cerr << "[LifecycleManager] destroy" << endl;
#endif
		for ( std::set< std::string >::iterator it = managed_instances->instances().begin(); it!=managed_instances->instances().end(); ++it ) {
			LifeCycle* life = static_cast<LifeCycle*>(raw_component_management->get(EndPoint(*it, "lifecycle")));
			if ( life ) life->destroy();
		}
	}
	
};

LCMP(LifecycleManager)
	L_CPP_PROVIDE(LifeCycle, lifecycle_manager) // This is _NOT_ the lifecycle management of this instance, this is the management of the lifecycle for the instances managed by this one !
	L_CPP_USE(NamespaceManagement, managed_instances)
	L_CPP_USE(RawComponentManagement, raw_component_management)
LEND

}

}
