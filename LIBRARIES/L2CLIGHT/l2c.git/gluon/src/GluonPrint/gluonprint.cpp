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

#include <charm++.h>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/print.h>

namespace llcmcpp {

namespace impl {

class GluonPrint :
		virtual public Print
{
public:
	
	GluonPrint ()	{}
	
	virtual void printout ( const std::string& msg)
	{
		CkPrintf("%s", msg.c_str());
	}

	virtual void printlnout ( const std::string& msg)
	{
		CkPrintf("%s\n", msg.c_str());
	}

	virtual void printerr ( const std::string& msg)
	{
		CkError("%s", msg.c_str());
	}

	virtual void printlnerr ( const std::string& msg)
	{
		CkError("%s\n", msg.c_str());
	}
};

LCMP(GluonPrint)
	L_CPP_PROVIDE(Print, print);
LEND

}

}
