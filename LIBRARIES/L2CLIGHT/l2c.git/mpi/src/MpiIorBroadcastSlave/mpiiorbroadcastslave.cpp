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

class MpiIorBroadcastSlave:
		virtual public Initializer
{
public:
	MPI_Comm comm;
	
	string* ior;
	
	MpiIorBroadcastSlave ():
			comm(MPI_COMM_NULL),
			ior(0)
	{}
	
    virtual void initialize(int* argc, char*** argv)
	{
		unsigned long size = 0;
		MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG, 0, comm);
		char* received_ior = new char[size+1]; // copy needed because string::data() returns a readonly pointer
		MPI_Bcast(received_ior, size+1, MPI_CHAR, 0, comm);
		received_ior[size] = 0;
		*ior = received_ior;
		delete[] received_ior;
	}
	
};

LCMP(MpiIorBroadcastSlave)
	L_CPP_PROVIDE(Initializer, initializer);
	L_MPI_COMM(comm);
	L_CPP_USE(string, ior);
LEND

}

}
