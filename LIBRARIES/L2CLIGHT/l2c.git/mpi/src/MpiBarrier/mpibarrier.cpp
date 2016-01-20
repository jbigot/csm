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
#include <list>
#include <vector>

#include <collective.h>
#include <llcmcpp3.h>
#include <llcmmpi.h>

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::vector;

namespace llcmcpp {

namespace impl {

class MpiBarrier:
		virtual public Collective
{
public:
	MPI_Comm comm;
	
	MpiBarrier ():
			comm(MPI_COMM_NULL)
	{}
	
	virtual void barrier ()
	{
		MPI_Barrier(comm);
	}
	
};

LCMP(MpiBarrier)
	L_CPP_PROVIDE(Collective, collective);
	L_MPI_COMM(comm);
LEND

}

}

