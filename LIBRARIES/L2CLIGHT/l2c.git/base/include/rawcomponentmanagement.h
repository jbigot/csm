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

#ifndef LLCMCPP_RAWCOMPONENTMANAGEMENT_H
#define LLCMCPP_RAWCOMPONENTMANAGEMENT_H

#include <string>

#include "endpoint.h"

namespace llcmcpp {

/*! \brief Provides a raw access to some component instances.
 */
class RawComponentManagement
{
public:
	/*! \brief Destroys a component instance.
	 * \param name the name (identifier) of the instance
	 * \pre id.state == DESTROYABLE
	 */
	virtual void destroy ( std::string name ) = 0;
	
	/*! \brief Provides raw acess to components input properties.
	 * \param property the property to access
	 * \param value a pointer to the value to set for the property, this should have the exact type expected by the component.
	 * \pre id.state == CREATED
	 */
	virtual void set ( EndPoint property, void* value ) = 0;
	
	/*! \brief Provides raw acess to components output properties.
	 * \param property the property to access
	 * \returns a pointer to the value of the property
	 * \pre id.state == CREATED | CONFIGURED | ACTIVATED
	 */
	virtual void* get ( EndPoint property ) = 0;
	
};

}

#endif // LLCMCPP_RAWCOMPONENTMANAGEMENT_H
