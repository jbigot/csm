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

#ifndef LLCMCPP_CLUSTERINSTANTIATIONMANAGEMENT_H
#define LLCMCPP_CLUSTERINSTANTIATIONMANAGEMENT_H

#include <string>

#include "hostid.h"

namespace llcmcpp {

class ClusterInstantiationManagement
{
public:
	/*! \brief Creates a new component instance in the cluser.
	 * \param host the host in the cluster where the instance should be created
	 * \param name the name (identifier) of the instance
	 * \param type the type of the instance
	 * \param from the library to load the instance from, defaults to $LD_LIBRARY_PATH+"lib"+type+".so"
	 * \post id.state == CREATED
	 */
	virtual void instantiate ( HostId host, std::string id, std::string type, std::string from = "" ) = 0;
	
};

}

#endif // LLCMCPP_CLUSTERINSTANTIATIONMANAGEMENT_H
