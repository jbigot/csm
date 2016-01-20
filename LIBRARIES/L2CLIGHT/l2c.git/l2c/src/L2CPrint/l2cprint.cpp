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

#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include "llcmcpp/print.h"

using ::std::cerr;
using ::std::endl;
using ::std::string;

namespace llcmcpp {

namespace impl {

class L2CPrint :
		virtual public Print
{
public:
	
	L2CPrint ()	{}
	
	virtual void printout ( const string& msg)
	{
	  ::std::cout << msg;
	}

	virtual void printlnout ( const string& msg)
	{
	  ::std::cout << msg << ::std::endl;
	}

	virtual void printerr ( const string& msg)
	{
	  ::std::cerr << msg;
	}

	virtual void printlnerr ( const string& msg)
	{
	  ::std::cerr << msg << ::std::endl;
	}
};

LCMP(L2CPrint)
	L_CPP_PROVIDE(Print, print);
LEND

}

}
