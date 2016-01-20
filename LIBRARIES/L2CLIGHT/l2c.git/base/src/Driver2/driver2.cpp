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

#include <list>

#include <go.h>
#include <initializer.h>
#include <llcmcpp3.h>

using ::std::list;

namespace llcmcpp {

namespace impl {

class Driver2:
		virtual public Go
{
public:
	int argc;
	
	char** argv;
	
	Go* assembly_building;
	
	void initializations ( Initializer* initialization )
	{
		m_initializers.push_back(initialization);
	}
	
	Driver2 ():
			assembly_building(0)
	{}
	
	virtual void go ()
	{
		for ( list< Initializer* >::iterator it = m_initializers.begin(); it != m_initializers.end(); ++it ) {
			(*it)->initialize(&argc, &argv);
		}
		
		assembly_building->go();
	}
	
private:
	list< Initializer* > m_initializers;
	
};

LCMP(Driver2)
	L_PROPERTY(int, argc)
	L_PROPERTY(char**, argv)
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(Go, assembly_building);
	L_CPP_USE(Initializer, initializations);
LEND

}

}

