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

#ifndef LLCMCPP_CLUSTERPLACEMENTMANAGEMENT_H
#define LLCMCPP_CLUSTERPLACEMENTMANAGEMENT_H

#include <set>
#include <string>

#include "hostid.h"

namespace llcmcpp {

/*! \brief Manages a (cluster local) namespace of component instances.
 */
class ClusterPlacementManagement
{
public:
	/*! \brief Retreives the host on which a given instance is hosted.
	 * \param instance the name (identifier) of the instance to look for
	 * \returns the identifier of the host for the instance
	 * \throws std::out_of_range if the instance doesn't exist
	 */
	virtual HostId get_rank ( std::string instance ) = 0;
	
};

}

#endif // LLCMCPP_CLUSTERPLACEMENTMANAGEMENT_H
