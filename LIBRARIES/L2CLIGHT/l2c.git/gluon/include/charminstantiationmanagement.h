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


#ifndef GLUON_CHARMINSTANTIATIONMANAGEMENT_H
#define GLUON_CHARMINSTANTIATIONMANAGEMENT_H

#include <string>

#include <stdint.h>

namespace gluon {

/*! \brief Instantition of Charm++ specific entities (groups, arrays & co.)
 */
class CharmInstantiationManagement
{
public:
	/*! \brief Creates a new group component instance.
	 * \param name the name (identifier) of the instance
	 * \param type the type of the instance
	 * \param from the library to load the instance from, defaults to $LD_LIBRARY_PATH+"/lib"+type+".so"
	 * \post id.state == CREATED
	 */
	virtual void instantiate_group ( std::string name, std::string type, std::string from = "" ) = 0;
	
	/*! \brief Creates a new array component instance.
	 * \param name the name (identifier) of the instance
	 * \param size the size (number of elements) of the instance
	 * \param type the type of the instance
	 * \param from the library to load the instance from, defaults to $LD_LIBRARY_PATH+"/lib"+type+".so"
	 * \post id.state == CREATED
	 */
	virtual void instantiate_array ( std::string name, uint32_t size, std::string type, std::string from = "" ) = 0;
	
	/*! \brief Creates a new array component instance.
	 * \param name the name (identifier) of the instance
	 * \param size the size (number of elements) of the instance
	 * \param bound_to another array instance this one is to be bound to
	 * \param type the type of the instance
	 * \param from the library to load the instance from, defaults to $LD_LIBRARY_PATH+"/lib"+type+".so"
	 * \post id.state == CREATED
	 */
	virtual void instantiate_bound_array ( std::string name, uint32_t size, std::string bound_to, std::string type, std::string from = "" ) = 0;
	
};

}

#endif // GLUON_CHARMINSTANTIATIONMANAGEMENT_H
