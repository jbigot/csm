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

#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <go.h>
#include <host.h>
#include <initializer.h>
#include <llcmcpp3.h>
#include <systemproperty.h>
#include <typesetter.h>
#include <xmlparser.h>

using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::exit;
using ::std::list;
using ::std::string;
using ::std::vector;

namespace llcmcpp {

namespace impl {

class Driver:
		virtual public Go
{
public:
	int argc;
	
	char** argv;
	
	Go* assembly_building;
	
	Host* host;
	
	void initializations ( Initializer* initialization )
	{
		m_initializers.push_back(initialization);
	}
	
	SystemProperty* system_properties;
	
	TypeSetter* typed_configuration;
	
	XmlParser* xml_parsing;
	
	Driver ():
			argc(0),
			argv(0),
			assembly_building(0),
			system_properties(0),
			typed_configuration(0),
			xml_parsing(0)
	{}
	
	virtual void go ()
	{
		for ( list< Initializer* >::iterator it = m_initializers.begin(); it != m_initializers.end(); ++it ) {
			(*it)->initialize(&argc, &argv);
		}
#ifndef NDEBUG
		cerr << "[Driver] Go" << endl;
		cerr << "[Driver] argc=" << argc << endl;
		cerr << "[Driver] argv=| ";
		for ( int ii=0; ii<argc; ++ii ) {
			cerr << argv[ii] << " | ";
		}
		cerr << endl;
#endif // NDEBUG
		if ( argc < 2 ) {
			cerr << "[Driver] *** Warning: no argument found!" << endl;
			cout << "Usage: " << argv[0] << " <cad file> [-D<instance>.<property>=<value>]* [-- <parameters for the assembly>]" << endl;
			exit(EXIT_SUCCESS);
		}
		
		int ii = 0;
		vector< const char* > new_args;
		
		// skip argv[0]
		++ii;
		
		// take care of the CAD file
		string cad_file = argv[ii];
		new_args.push_back(cad_file.c_str());
		++ii;
		
		// handle -D<instance>.<property>=<value> args
		for(; ii<argc; ii++) {
			string arg = argv[ii];
			if ( arg == "--" ) break; // not our parameters anymore
			if ( arg.substr(0, 2) != "-D" ) {
				cerr << "[Driver] *** Warning: unrecognized command line element: \"" << arg << "\" passing it as is!" << endl;
				new_args.push_back(argv[ii]);
				continue;
			}
			size_t dot = arg.find(".", 2);
			size_t eq = arg.find("=", dot);
			
			typed_configuration->configure(EndPoint(arg.substr(2, dot-2), arg.substr(dot+1, eq-dot-1)), "string", arg.substr(eq+1));
		}
		
		++ii; // skip the "--"
		// forward the remaining
		if ( ii<argc) new_args.insert(new_args.end(), argv+ii, argv+argc);
		
		
		// set "argc", "argv" & "host" system properties
		int new_argc = new_args.size(); // should last until end of execution
		system_properties->set_property("argc", &new_argc);
		const char** new_argv = new_args.data();
		system_properties->set_property("argv", &new_argv);
		system_properties->set_property("host", host);
		
		xml_parsing->process_file(cad_file.c_str());
		
		assembly_building->go();
	}
	
private:
	list< Initializer* > m_initializers;
	
};

LCMP(Driver)
	L_PROPERTY(int, argc)
	L_PROPERTY(char**, argv)
	L_CPP_PROVIDE(Go, go)
	L_CPP_USE(Go, assembly_building)
	L_CPP_USE(Host, host)
	L_CPP_USE(Initializer, initializations)
	L_CPP_USE(SystemProperty, system_properties)
	L_CPP_USE(TypeSetter, typed_configuration)
	L_CPP_USE(XmlParser, xml_parsing)
LEND

}

}

