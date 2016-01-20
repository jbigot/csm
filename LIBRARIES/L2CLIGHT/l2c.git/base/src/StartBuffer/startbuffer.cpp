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

#include <complex>
#include <iostream>
#include <list>
#include <string>

#include <start.h>
#include <go.h>
#include <llcmcpp3.h>

namespace llcmcpp {

namespace impl {

using ::std::cerr;
using ::std::endl;
using ::std::list;
using ::std::string;

class StartBuffer:
		virtual public Start,
		virtual public Go
{
public:
	Start* start_management;
	
	StartBuffer ():
			start_management(0)
	{}
	
	virtual void start ( EndPoint property )
	{
		m_buffer.push_back(Start(property));
	}
	
	virtual void go ()
	{
#ifndef NDEBUG
		cerr << "[StartBuffer] executing!" << endl;
#endif
		for ( list< Start >::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it ) {
			start_management->start(it->property);
		}
		m_buffer.clear();
#ifndef NDEBUG
		cerr << "[StartBuffer] executions done!" << endl;
#endif
	}
	
private:
	struct Start
	{
		EndPoint property;
		Start ( EndPoint property ): property(property) {}
	};
	
	list< Start > m_buffer;
	
};

LCMP(StartBuffer)
	L_CPP_PROVIDE(Start, start_manager);
	L_CPP_PROVIDE(Go, go);
	L_CPP_USE(Start, start_management);
LEND

}

}

