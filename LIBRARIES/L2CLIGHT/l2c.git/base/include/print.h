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

#ifndef LLCMCPP_PRINT_H
#define LLCMCPP_PRINT_H

#include <string>
#include <iostream>

namespace llcmcpp
{

class Print
{
public:
	virtual void printout (const ::std::string&) = 0;
	virtual void printlnout (const ::std::string&) = 0;
	virtual void printerr (const ::std::string&) = 0;
	virtual void printlnerr (const ::std::string&) = 0;
};

}

#define PRINTOUT(port, msg)   { std::stringstream str; str << msg; if (port) { port->printout(str.str());   } else { std::cout<<str.str(); }}
#define PRINTLNOUT(port, msg) { std::stringstream str; str << msg; if (port) { port->printlnout(str.str()); } else { std::cout<<str.str() << "\n"; }}
#define PRINTERR(port, msg)   { std::stringstream str; str << msg; if (port) { port->printerr(str.str());   } else { std::cerr<<str.str(); }}
#define PRINTLNERR(port, msg) { std::stringstream str; str << msg; if (port) { port->printlnerr(str.str()); } else { std::cerr<<str.str() << "\n"; }}

#endif // LLCMCPP_PRINT_H
