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

#ifndef LLCMCPP_HOST3_H
#define LLCMCPP_HOST3_H

#include <stdint.h>
#include <string>

namespace llcmcpp
{

/*! \brief a Host3 manages a set of up to 2^64 component instances in one process.
 */
class Host3
{
public:
	/*! \brief a opaque handle identifying component instances managed by this host
	 */
	typedef uint64_t ComponentInstance;

	/*! \brief Instanciates a component
	 * \param type the type of the component to instanciate
	 * \param library path to the library from which to load the component, defaults to $LD_LIBRARY_PATH+"lib"+type+".so"
	 * \return an opaque handle on the component instance
	 */
	virtual ComponentInstance instantiate ( std::string type, std::string library = "" ) = 0;

	/*! \brief Destroys a given component instance
	 * Destroys an instance: i.e. calls its destructor and deallocates the memory.
	 * \param instance the component instance identifier
	 */
	virtual void destroy ( ComponentInstance instance ) = 0;

	/*! \brief Gets a pointer to the value of a component's property
	 * \param instance the component instance identifier
	 * \param propname the name on the property
	 * \return the value of the property
	 */
	virtual void* get ( ComponentInstance instance, std::string propname ) = 0;

	/*! \brief Sets the value of a component's property
	 * \param instance the component instance identifier
	 * \param propname the name on the property
	 * \param value a pointer to the value to set for the property
	 */
	virtual void set ( ComponentInstance instance, std::string propname, void* value ) = 0;

	/*! \brief Preloads a component type.
	 * \param type the type of the component to preload
	 * \param library path to the library from which to load the component, defaults to $LD_LIBRARY_PATH+"lib"+type+".so"
	 */
	virtual void preload ( std::string type, std::string library = "" ) = 0;

};

}

#endif // LLCMCPP_HOST3_H
// kate: indent-mode cstyle; indent-width 2; replace-tabs off; tab-width 2;
