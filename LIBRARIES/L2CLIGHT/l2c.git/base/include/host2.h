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

#ifndef LLCMCPP_HOST2_H
#define LLCMCPP_HOST2_H

#include "host.h"

namespace llcmcpp {

class Host2:
		virtual public Host
{
public:
	/*! \brief Preloads a component type.
	 * \param type the type of the component to preload
	 * \param library path to the library from which to load the component, defaults to $LD_LIBRARY_PATH+"lib"+type+".so"
	 */
	virtual void preload ( std::string type, std::string library = "" ) = 0;
	
};

}

#endif // LLCMCPP_HOST2_H
