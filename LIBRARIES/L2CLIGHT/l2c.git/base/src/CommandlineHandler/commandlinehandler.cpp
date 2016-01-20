/*
    This file is part of Gluon.
    Gluon is a component model build on top of Charm++
    Copyright © 2011  Julien Bigot <julien.bigot@inria.fr>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

#include <initializer.h>
#include <llcmcpp3.h>
#include <typesetter.h>
#include <xmlparser.h>

namespace llcmcpp {

namespace impl {

using ::boost::unordered_map;
using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::exit;
using ::std::make_pair;
using ::std::string;
using ::std::vector;

class CommandlineHandler:
		virtual public Initializer
{
public:
	vector< const char* > args;
	
	TypeSetter* typed_configuration;
	
	XmlParser* xml_parsing;
	
	CommandlineHandler ():
			typed_configuration(0),
			xml_parsing(0)
	{}
	
	virtual void initialize ( int* argc, char*** argv )
	{
#ifndef NDEBUG
		cerr << "[CommandlineHandler] Go" << endl;
		cerr << "[CommandlineHandler] argc=" << *argc << endl;
		cerr << "[CommandlineHandler] argv=| ";
		for ( int ii=0; ii<(*argc); ++ii ) {
			cerr << (*argv)[ii] << " | ";
		}
		cerr << endl;
#endif // NDEBUG
		if ( *argc < 2 ) {
			cerr << "[CommandlineHandler] *** Warning: no argument found!" << endl;
			cout << "Usage: " << (*argv)[0] << " [-D<instance>.<property>=<value>]* [--] <cad file> [<parameters for the assembly>]*" << endl;
			exit(EXIT_SUCCESS);
		}
		
		// start with a fake NULL CAD name
		args.push_back(0);
		
		bool end_found = false;
		for(int ii=1; ii<(*argc); ii++) { // handle args (except *argv[0] that is the prog name)
			string string_arg = (*argv)[ii];
			if ( !end_found && string_arg == "--" ) { // parameter list end
				end_found = true;
			} else if ( string_arg.substr(0, 2) == "-D" && !end_found ) {// handle -D<instance>.<property>=<value> args
				size_t dot = string_arg.find(".", 2);
				size_t eq = string_arg.find("=", dot);
				if ( args[0] ) {
					cerr << "[CommandlineHandler] *** Warning: setting property "<<string_arg.substr(dot+1, eq-dot-1)<<" of "<<string_arg.substr(2, dot-2)<<", use -- to pass it as assembly argument!" << endl;
				}
				typed_configuration->configure(EndPoint(string_arg.substr(2, dot-2), string_arg.substr(dot+1, eq-dot-1)), "string", string_arg.substr(eq+1));			
			} else if ( (string_arg.substr(0, 1) != "-" || end_found) && !args[0] ) { // handle CAD name
				args[0] = (*argv)[ii];
			} else if ( end_found || args[0] ) { // handle CAD params
				args.push_back((*argv)[ii]);
			} else {
				cerr << "[CommandlineHandler] *** Warning: unrecognized command line element: \"" << (*argv)[ii] << "\" passing it as is!" << endl;
				args.push_back((*argv)[ii]);
			}
		}
		
		if ( !args[0] ) {
			cerr << "[CommandlineHandler] *** Error: no CAD name found!" << endl;
			cout << "Usage: " << (*argv)[0] << " <cad file> [-D<instance>.<property>=<value>]* [-- <parameters for the assembly>]" << endl;
			exit(1);
		}
		
		xml_parsing->process_file(args[0]);
	}
};

LCMP(CommandlineHandler)
	L_CPP_PROVIDE(Initializer, initializer);
	L_CPP_PROVIDE2(vector< const char* >, args);
	L_CPP_USE(TypeSetter, typed_configuration);
	L_CPP_USE(XmlParser, xml_parsing);
LEND

}

}
