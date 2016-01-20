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

#ifndef LLCMCPP_CPPMANAGEMENT2_H
#define LLCMCPP_CPPMANAGEMENT2_H

#include <complex>

#include <stdint.h>

#include "endpoint.h"

namespace llcmcpp {

/*! \brief manages the local (C++) aspects of an LLCMc++ assembly
 * Unlike CppManagement, this interface does no handle the starting of
 * component instances that should be done through the start interface.
 */
class CppManagement2
{
public:
	/*! \brief connects two components by a C++ use/provide interaction
	 * \param user the user endpoint
	 * \param provider the provider endpoint
	 * \pre user.state == created && provider.state == created
	 */
	virtual void connect ( EndPoint user, EndPoint provider ) = 0;
	
	/*! \brief Configures an input property of a component using a system value.
	 * \param property the property to configure
	 * \param value_name the name of the system value to use
	 * \pre id.state == created
	 */
	virtual void system_configure ( EndPoint property, std::string value_name ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, int8_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, uint8_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, int16_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, uint16_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, int32_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, uint32_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, int64_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, uint64_t value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, double value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, std::complex<double> value ) = 0;
	
	/*! \brief Configures an input property of a component.
	 * \param property the property to configure
	 * \param provider the value to set for the property
	 * \pre id.state == created
	 */
	virtual void configure ( EndPoint property, std::string value ) = 0;

};

}

#endif // LLCMCPP_CPPMANAGEMENT2_H
