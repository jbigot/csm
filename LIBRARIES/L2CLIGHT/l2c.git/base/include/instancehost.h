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

#ifndef LLCMCPP_IMPL_LOCALHOSTCOMPONENT_H
#define LLCMCPP_IMPL_LOCALHOSTCOMPONENT_H

#include <map>
#include <string>
#include <vector>

#include "component.h"
#include "host2.h"
#include "host3.h"
#include "print.h"

namespace llcmcpp
{

class Component;

class InstanceHost :
		virtual public Host3
{
public:
	InstanceHost ();

	~InstanceHost ();

	// ports

	Print* print;

	Host* host () { return &m_host_legacy; };

	Host2* host2 () { return &m_host_legacy; };

	// Host

	virtual ComponentInstance instantiate ( std::string type, std::string from = "" );

	virtual void preload ( std::string type, std::string from = "" );

	virtual void destroy ( ComponentInstance instance );

	virtual void* get ( ComponentInstance instance, std::string port );

	virtual void set ( ComponentInstance instance, std::string port, void* value );

private:
	ComponentFactory factory ( std::string name, std::string from );

	std::map< std::string, llcmcpp::ComponentFactory > m_factories;

	void* handle ( std::string library );

	std::map< std::string, void* > m_handles;

	ComponentInstance m_available_compid;

	std::map< ComponentInstance, llcmcpp::Component* > m_components;

	class Host2Adapter :
			public Host2
	{
	public:
		Host2Adapter(InstanceHost& container):
				m_container(container)
		{}

		virtual ComponentInstance instantiate ( std::string type, std::string from = "" ) {
			return m_container.InstanceHost::instantiate ( type,  from );
		}

		virtual void preload ( std::string type, std::string from = "" ) {
			m_container.InstanceHost::preload ( type,  from );
		}

		virtual void destroy ( ComponentInstance instance ) {
			m_container.InstanceHost::destroy ( instance );
		}

		virtual void* get ( ComponentInstance instance, std::string port ) {
			return m_container.InstanceHost::get ( instance,  port );
		}

		virtual void set ( ComponentInstance instance, std::string port, void* value ) {
			m_container.InstanceHost::set ( instance,  port, value );
		}

	private:
		InstanceHost& m_container;

	} m_host_legacy;

};

}

#endif // LLCMCPP_IMPL_LOCALHOSTCOMPONENT_H
