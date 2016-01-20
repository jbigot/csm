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
#include <cstdlib>
#include <sstream>

#include <cppmanagement.h>
#include <llcmcpp3.h>
#include <typesetter.h>
#include <cppmanagement2.h>

using ::std::cerr;
using ::std::complex;
using ::std::endl;
using ::std::string;
using ::std::stringstream;

namespace llcmcpp {

namespace impl {

class TypeInterpreter :
		virtual public TypeSetter
{
public:
	TypeInterpreter():
			cpp_management(0),
			cpp_management2(0)
	{
	}
	
	CppManagement* cpp_management;
	
	CppManagement2* cpp_management2;
	
	
	
	virtual void configure ( EndPoint property, string type, string value )
	{
		stringstream valstr(value);
		
		if ( type == "int8" ) {
			int8_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "uint8" ) {
			uint8_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "int16" ) {
			int16_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "uint16" ) {
			uint16_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "int32" ) {
			int32_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "uint32" ) {
			uint32_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "int64" ) {
			int64_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "uint64" ) {
			uint64_t tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "double" ) {
			double tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "complex double" ) {
			complex< double > tval;
			valstr >> tval;
			if ( valstr ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (" << type << ") " << value << endl;
#endif // NDEBUG
				if ( cpp_management) {
					cpp_management->configure(property, tval);
				} else {
					cpp_management2->configure(property, tval);
				}
			} else {
				cerr << "[TypeInterpreter] ** ERROR: Invalid value of type " << type << ": " << value << endl;
				abort();
			}
		} else if ( type == "string" ) {
#ifndef NDEBUG
				cerr << "[TypeInterpreter] " << property.instance << '.' << property.port << " := (string) " << value << endl;
#endif // NDEBUG
			if ( cpp_management) {
				cpp_management->configure(property, value);
			} else {
				cpp_management2->configure(property, value);
			}
		} else {
			cerr << "[TypeInterpreter] ** ERROR: Unknown type \"" << type << '"' << endl;
			abort();
		}
	}
	
};

LCMP(TypeInterpreter)
	L_CPP_PROVIDE(TypeSetter, interpreter)
	L_CPP_USE(CppManagement, cpp_management)
	L_CPP_USE(CppManagement2, cpp_management2)
LEND

}

}
