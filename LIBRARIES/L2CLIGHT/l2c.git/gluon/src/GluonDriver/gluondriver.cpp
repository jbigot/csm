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
#include <cstring>
#include <list>

#include <llcmcpp/assemblybuilding.h>
#include <llcmcpp/go.h>
#include <llcmcpp/initializer.h>
#include <llcmcpp/llcmcpp3.h>

#include <asynccollective.h>

namespace gluon {

namespace impl {

using ::llcmcpp::AssemblyBuilding;
using ::llcmcpp::Go;
using ::llcmcpp::Initializer;
using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::strcpy;
using ::std::strlen;

class GluonDriver:
		virtual public Initializer,
		virtual public Go,
		virtual private Callback
{
public:
	AssemblyBuilding* assembly_builder;
	
	AsyncCollective* collective;
	
	GluonDriver ():
			assembly_builder(0),
			collective(0),
			m_argc(0),
			m_argv(0),
			m_step(INST)
	{}
	
	void initializations ( Initializer* initialization )
	{
		m_initializers.push_back(initialization);
	}
	
	void runtime_initializations ( Initializer* runtime_initialization )
	{
		m_runtime_initializers.push_back(runtime_initialization);
	}
	
	virtual void initialize ( int* pargc, char*** pargv)
	{
		m_argc = *pargc;
		m_argv = new char*[m_argc];
		for ( int ii = 0; ii<m_argc; ++ii ) {
			m_argv[ii] = new char[strlen((*pargv)[ii])+1];
			strcpy(m_argv[ii], (*pargv)[ii]);
		}
		for ( list< Initializer* >::iterator it = m_initializers.begin(); it != m_initializers.end(); ++it ) {
			(*it)->initialize(&m_argc, &m_argv);
		}
	}
	
	virtual void go ()
	{
		for ( list< Initializer* >::iterator it = m_runtime_initializers.begin(); it != m_runtime_initializers.end(); ++it ) {
			(*it)->initialize(&m_argc, &m_argv);
		}
		operator()();
	}
	
private:
	virtual void operator() ()
	{
#ifndef NDEBUG
		cerr << "[GluonDriver] operator("<<m_step<<")"<<endl;
#endif
		switch ( m_step ) {
		case INST:
			assembly_builder->instantiation();
			m_step = CONF;
			collective->barrier(*this);
			break;
		case CONF:
			assembly_builder->configuration();
			m_step = ACTIV;
			collective->barrier(*this);
			break;
		case ACTIV:
			assembly_builder->activation();
			m_step = EXEC;
			collective->barrier(*this);
			break;
		case EXEC:
			assembly_builder->execution();
			m_step = DEACT;
			collective->barrier(*this);
			break;
		case DEACT:
			assembly_builder->deactivation();
			m_step = DESTR;
			collective->barrier(*this);
			break;
		case DESTR:
			assembly_builder->destruction();
			m_step = INST;
			break;
		}
	}
	
	int m_argc;
	
	char** m_argv;
	
	enum { INST, CONF, ACTIV, EXEC, DEACT, DESTR } m_step;
	
	list< Initializer* > m_initializers;
	
	list< Initializer* > m_runtime_initializers;
	
};

LCMP(GluonDriver)
	L_CPP_PROVIDE(Initializer, init);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(AssemblyBuilding, assembly_builder);
	L_CPP_USE(AsyncCollective, collective);
	L_CPP_USE(Initializer, initializations);
	L_CPP_USE(Initializer, runtime_initializations);
LEND

}

}


