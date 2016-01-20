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

#include <mpi.h>

#include <iostream>
#include <string>

#include <initializer.h>
#include <llcmcpp3.h>
#include <llcmmpi.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::string;

class MpiIorBroadcastRoot:
		virtual public Initializer
{
public:
	MPI_Comm comm;
	
	string* ior;
	
	MpiIorBroadcastRoot ():
			comm(MPI_COMM_NULL),
			ior(0)
	{}
	
    virtual void initialize(int* argc, char*** argv)
	{
		unsigned long size = ior->size();
		MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG, 0, comm);
		MPI_Bcast(const_cast<char*>(ior->c_str()), size+1, MPI_CHAR, 0, comm);
	}
	
};

LCMP(MpiIorBroadcastRoot)
	L_CPP_PROVIDE(Initializer, initializer);
	L_MPI_COMM(comm);
	L_CPP_USE(string, ior);
LEND

}

}
