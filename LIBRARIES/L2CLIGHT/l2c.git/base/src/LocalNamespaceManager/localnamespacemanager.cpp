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
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include <tinyxml.h>

#include <go.h>
#include <host.h>
#include <host3.h>
#include <llcmcpp3.h>
#include <localinstantiationmanagement.h>
#include <namespacemanagement.h>
#include <rawcomponentmanagement.h>

using ::std::cerr;
using ::std::endl;
using ::std::map;
using ::std::out_of_range;
using ::std::set;
using ::std::string;

namespace llcmcpp {

namespace impl {

class LocalNamespaceManager :
		virtual public LocalInstantiationManagement,
		virtual public NamespaceManagement,
		virtual public RawComponentManagement
{
public:
	Host* host;

	Host3* host3;

	LocalNamespaceManager ():
		host(0),
		host3(0)
	{}
	
	virtual void instantiate ( string name, string type, string from )
	{
#ifndef NDEBUG
		cerr << "[LocalNamespaceManager] Creating instance id=" << name << " type=" << type << " from \"" << from << '"' << endl;
#endif
		Host3::ComponentInstance inst;
		if ( host3 ) {
			inst = host3->instantiate(type, from);
		} else {
			inst = host->instantiate(type, from);
		}
		if ( ! m_ids.insert(make_pair(name, inst)).second ) {
			cerr << "[LocalNamespaceManager] ** Error: duplicate component id: \"" << name << '"' << endl;
			abort();
		}
		m_instances.insert(name);
	}
	
	virtual void destroy ( string name )
	{
#ifndef NDEBUG
		cerr << "[LocalNamespaceManager] Destroying instance " << name << endl;
#endif
		try {
			if ( host3 ) {
				host3->destroy(m_ids.at(name));
			} else {
				host->destroy(m_ids.at(name));
			}
		} catch ( const out_of_range& e ) {
			cerr << "[LocalNamespaceManager] ** Warning: " << name << " does not exist" << endl;
			throw e;
		}
		m_instances.erase(name);
	}
	
	virtual void set ( EndPoint property, void* value )
	{
#ifndef NDEBUG
		cerr << "[LocalNamespaceManager] set " << property.instance << "." << property.port << endl;
#endif
		try {
			if ( host3 ) {
				host3->set(m_ids.at(property.instance), property.port, value);
			} else {
				host->set(m_ids.at(property.instance), property.port, value);
			}
		} catch ( const out_of_range& e ) {
			cerr << "[LocalNamespaceManager] ** Warning: " << property.instance << " does not exist" << endl;
			throw e;
		}
	}
	
	virtual void* get ( EndPoint property )
	{
#ifndef NDEBUG
		cerr << "[LocalNamespaceManager] get " << property.instance << "." << property.port << endl;
#endif
		try {
			if ( host3 ) {
				return host3->get(m_ids.at(property.instance), property.port);
			} else {
				return host->get(m_ids.at(property.instance), property.port);
			}
		} catch ( const out_of_range& e ) {
			cerr << "[LocalNamespaceManager] ** Warning: " << property.instance << " does not exist" << endl;
			throw e;
		}
	}
	
	virtual const std::set< string >& instances()
	{
		return m_instances;
	}
	
private:
	map< string, Host3::ComponentInstance > m_ids;
	
	std::set< string > m_instances;
	
};

LCMP(LocalNamespaceManager)
	L_CPP_PROVIDE(LocalInstantiationManagement, instantiation_manager);
	L_CPP_PROVIDE(NamespaceManagement, namespace_management);
	L_CPP_PROVIDE(NamespaceManagement, namespace_manager);
	L_CPP_PROVIDE(RawComponentManagement, raw_component_manager);
	L_CPP_USE(Host, host);
	L_CPP_USE(Host3, host3);
LEND

}

}
