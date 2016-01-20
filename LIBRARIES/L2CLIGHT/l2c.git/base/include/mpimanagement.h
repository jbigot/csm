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

#ifndef LLCMCPP_MPIMANAGEMENT_H
#define LLCMCPP_MPIMANAGEMENT_H

#include <list>

#include "endpoint.h"

namespace llcmcpp {

/*! \brief manages the cluster-wide (MPI) aspects of an LLCMc++ assembly
 */
class MpiManagement
{
public:
	/*! \brief connects some components with an MPI communicator.
	 * \warning There can be at most one instance so connected by process.
	 * \param group the endpoints to be configured with the communicator
	 * \pre each(endpoint:group) { endpoint.state == created }
	 */
	virtual void mpi_connect ( std::list< EndPoint > group ) = 0;
	
};

}

#endif // LLCMCPP_MPIMANAGEMENT_H
