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
#include <list>
#include <string>

#include <go.h>
#include <llcmcpp3.h>
#include <mpimanagement.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class MpiManagementBuffer:
		virtual public Go,
		virtual public MpiManagement
{
public:
	MpiManagement* mpi_management;
	
	MpiManagementBuffer ():
			mpi_management(0)
	{}
	
    virtual void mpi_connect( list< EndPoint > group )
    {
		m_configurations.push_back(group);
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[MpiManagementBuffer] configurating!" << endl;
#endif
		for ( list< list< EndPoint > >::iterator it = m_configurations.begin(); it != m_configurations.end(); ++it ) {
			mpi_management->mpi_connect(*it);
		}
		m_configurations.clear();
#ifndef NDEBUG
		cerr << "[MpiManagementBuffer] configuration done!" << endl;
#endif
	}
	
private:
	list< list< EndPoint > > m_configurations;
	
};

LCMP(MpiManagementBuffer)
	L_CPP_PROVIDE(Go, go);
	L_CPP_PROVIDE(MpiManagement, mpi_manager);
	L_CPP_USE(MpiManagement, mpi_management);
LEND

}

}
