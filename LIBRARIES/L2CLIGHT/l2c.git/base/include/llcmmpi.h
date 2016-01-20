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

#ifndef LLCMCPP_LLCMMPI_H
#define LLCMCPP_LLCMMPI_H

#include <mpi.h>

#include "llcmcommon.h"

#include "component.h"

namespace llcmcpp {

namespace impl {

template < class Cmp >
struct SetterMpiComm:
		public Setter< Cmp >
{
	SetterMpiComm ( void (Cmp::*mth) ( MPI_Comm ) ):
			m_mth(mth)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		(c->*m_mth)(*static_cast< MPI_Comm* >(v));
	}
	
	void (Cmp::*m_mth) ( MPI_Comm );
	
};

template < class Cmp >
struct AttributeMpiComm:
		public Setter< Cmp >
{
	AttributeMpiComm ( MPI_Comm Cmp::*dat ):
			m_dat(dat)
	{}
	
	virtual void operator() ( Cmp* c, void* v )
	{
		c->*m_dat = *static_cast< MPI_Comm* >(v);
	}
	
	MPI_Comm Cmp::*m_dat;
	
};

template < class Cmp >
void mpi_comm ( BasicComponent< Cmp >* cmp, std::string name, MPI_Comm Cmp::*dat )
{
	cmp->input_properties[name] = new AttributeMpiComm< Cmp >(dat);
}

}

}

#define L_MPI_COMM(name) \
	::llcmcpp::impl::mpi_comm< Cmp >(cmp, #name, &Cmp::name);

#endif // LLCMCPP_LLCMMPI_H
