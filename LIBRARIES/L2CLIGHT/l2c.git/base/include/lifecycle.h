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

#ifndef LLCMCPP_LIFECYCLE_H
#define LLCMCPP_LIFECYCLE_H

namespace llcmcpp
{

/*! \brief Manages the life cycle of component instances.
 * The life cycle of components is represented by a state machine with the following states: INEXISTANT, CREATED, CONFIGURED, ACTIVATED.
 * When it's build using its constructor, a component is in the CREATED state.
 * The CONFIGURED state means that no more "set" should be called on the component and that it should be ready for access to the properties it has exposed.
 * The ACTIVATED state means that all components are at least CONFIGURED and that this instance can start using the properties it imports.
 * The DESTROYABLE state is deprecated, it has no clearly defined meaning except for the fact that the destructor is likely to be called soon.
 */
class LifeCycle
{
public:
	/*! \brief The instance has been configured: no more set pending, should be ready for access to its ports
	 * Notifies the instance that the state has changed from CREATED to CONFIGURED.
	 */
	virtual void configure_complete () {}
   
	/*! \brief All components have been configured: can start using ports
	 * Notifies the instance that the state has changed from CONFIGURED to ACTIVATED.
	 */
	virtual void activate () {}
	
	/*! \brief This component has to stop: can not use ports anymore
	 * Notifies the instance that the state has changed from ACTIVATED to CONFIGURED.
	 */
	virtual void deactivate () {}
	
	/*! \brief This component will soon be destroyed.
	 * \deprecated you should rely on the destructor instead.
	 */
	virtual void destroy () {}
	
};

}

#endif // LLCMCPP_LIFECYCLE_H
