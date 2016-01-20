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
#include <string>
#include <vector>

#include <assemblybuilding.h>
#include <collective.h>
#include <go.h>
#include <lifecycle.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::string;
using ::std::vector;

class AssemblyBuilder2:
		virtual public AssemblyBuilding,
		virtual public Go
{
public:
	Collective* collective_access;
	
	LifeCycle* lifecycle_management;
	
	void instantiations ( Go* instantiation )
	{
		m_instantiations.push_back(instantiation);
	}
	
	vector< Go* > m_instantiations;
	
	void configurations ( Go* configuration )
	{
		m_configurations.push_back(configuration);
	}
	
	vector< Go* > m_configurations;
	
	void executions ( Go* execution )
	{
		m_executions.push_back(execution);
	}
	
	vector< Go* > m_executions;
	
	AssemblyBuilder2 ():
			collective_access(0),
			lifecycle_management(0)
	{}
	
	virtual void instantiation ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] instantiating!" << endl;
#endif
		for ( int ii = 0; ii < m_instantiations.size(); ++ii ) {
			m_instantiations[ii]->go();
		}
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] instantiation done!" << endl;
#endif
	}
	
	virtual void configuration ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] configurating!" << endl;
#endif
		for ( int ii = 0; ii < m_configurations.size(); ++ii ) {
			m_configurations[ii]->go();
		}
		lifecycle_management->configure_complete();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] configuration done!" << endl;
#endif
	}
	
	virtual void activation ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] activating!" << endl;
#endif
		lifecycle_management->activate();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] activation done!" << endl;
#endif
	}
	
	virtual void execution ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] executing!" << endl;
#endif
		for ( int ii = 0; ii < m_executions.size(); ++ii ) {
			m_executions[ii]->go();
		}
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] executions done!" << endl;
#endif
	}
	
	virtual void deactivation ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] deactivating!" << endl;
#endif
		lifecycle_management->deactivate();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] deactivation done!" << endl;
#endif
	}
	
	virtual void destruction ()
	{
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] destroying!" << endl;
#endif
		lifecycle_management->destroy();
#ifndef NDEBUG
		cerr << "[AssemblyBuilder2] destruction done!" << endl;
#endif
	}
	
	virtual void go ()
	{
		instantiation();
		if ( collective_access ) collective_access->barrier();
		configuration();
		if ( collective_access ) collective_access->barrier();
		activation();
		if ( collective_access ) collective_access->barrier();
		execution();
		if ( collective_access ) collective_access->barrier();
		deactivation();
		if ( collective_access ) collective_access->barrier();
		destruction();
	}
};

LCMP(AssemblyBuilder2)
	L_CPP_PROVIDE(AssemblyBuilding, assembly_building);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(Go, instantiations);
	L_CPP_USE(Go, configurations);
	L_CPP_USE(Go, executions);
	L_CPP_USE(Collective, collective_access);
	L_CPP_USE(LifeCycle, lifecycle_management);
LEND

}

}

