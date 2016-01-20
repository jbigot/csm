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

#ifndef LLCMCPP_ASSEMBLYBUILDING_H
#define LLCMCPP_ASSEMBLYBUILDING_H

namespace llcmcpp {

class AssemblyBuilding
{
public:
	virtual void instantiation () = 0;
	
	virtual void configuration () = 0;
	
	virtual void activation () = 0;
	
	virtual void execution () = 0;
	
	virtual void deactivation () = 0;
	
	virtual void destruction () = 0;
	
};

}

#endif // LLCMCPP_ASSEMBLYBUILDING_H
