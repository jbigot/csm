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

#ifdef WITH_MPI
#include <mpi.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>

#include <llcmcpp/go.h>
#include <llcmcpp/host2.h>
#include <llcmcpp/host3.h>
#include <llcmcpp/lifecycle.h>
#include <llcmcpp/systemproperty.h>
#include <llcmcpp/xmlparser.h>
#include <llcmcpp/instancehost.h>
#include <llcmcpp/print.h>

using ::llcmcpp::Go;
using ::llcmcpp::Host;
using ::llcmcpp::Host3;
using ::llcmcpp::InstanceHost;
using ::llcmcpp::LifeCycle;
using ::llcmcpp::SystemProperty;
using ::llcmcpp::XmlParser;
using ::std::cout;
using ::std::endl;

int main(int argc, char **argv)
{
  //int required = MPI_THREAD_SINGLE;
  //int provided;
  //MPI_Init_thread(&argc, &argv, required, &provided);
  MPI_Init(&argc,&argv);
  //std::cout<<"required serialized "<<required<<" - provided "<<provided<<std::endl;
	
	InstanceHost host;

	Host3::ComponentInstance l2cprint = host.instantiate("L2CPrint");
	host.print = static_cast<llcmcpp::Print*>(host.get(l2cprint, "print"));
	
	Host3::ComponentInstance namespace_manager = host.instantiate("LocalNamespaceManager");
	Host3::ComponentInstance lifecycle_manager = host.instantiate("LifecycleManager");
	Host3::ComponentInstance cpp_manager = host.instantiate("CppManager");
	Host3::ComponentInstance type_interpreter = host.instantiate("TypeInterpreter");
	Host3::ComponentInstance instantiation_filter = host.instantiate("LocalInstantiationFilter");
	Host3::ComponentInstance assembly_builder = host.instantiate("AssemblyBuilder");
	//Host3::ComponentInstance xml_handler = host.instantiate("LadHandler");
	Host3::ComponentInstance xml_parser = host.instantiate("Assembly");//TinyXmlComponent");
	Host3::ComponentInstance commandline_handler = host.instantiate("CommandlineHandler");
	Host3::ComponentInstance driver = host.instantiate("Driver2");
	
	host.set(namespace_manager, "host3", static_cast<Host3*>(&host));
	
	host.set(lifecycle_manager, "raw_component_management", host.get(namespace_manager, "raw_component_manager"));
	host.set(lifecycle_manager, "managed_instances", host.get(namespace_manager, "namespace_management"));
	
	host.set(cpp_manager, "args", host.get(commandline_handler, "args"));
	host.set(cpp_manager, "host3", static_cast<Host3*>(&host));
	host.set(cpp_manager, "host", host.host());
	host.set(cpp_manager, "raw_component_management", host.get(namespace_manager, "raw_component_manager"));
	
	host.set(type_interpreter, "cpp_management", host.get(cpp_manager, "cpp_manager"));
	
	host.set(instantiation_filter, "instantiation_management", host.get(namespace_manager, "instantiation_manager"));
	
	host.set(assembly_builder, "distributed_instantiation_management", host.get(instantiation_filter, "instantiation_manager"));
	host.set(assembly_builder, "lifecycle_management", host.get(lifecycle_manager, "lifecycle_manager"));
	host.set(assembly_builder, "cpp_management", host.get(cpp_manager, "cpp_manager"));
	host.set(assembly_builder, "typed_configuration", host.get(type_interpreter, "interpreter"));
	
	host.set(xml_parser/*handler*/, "grid_instantiation_management", host.get(assembly_builder, "distributed_instantiation_manager"));
	host.set(xml_parser, "cpp_management", host.get(assembly_builder, "cpp_manager"));
	host.set(xml_parser, "typed_configuration", host.get(assembly_builder, "typed_configurer"));
	//host.set(xml_handler, "grid_instantiation_management", host.get(instantiation_filter, "grid_instantiation_manager"));
	
	//host.set(xml_parser, "visitor", host.get(xml_handler, "visitor"));
	
	host.set(commandline_handler, "typed_configuration", host.get(assembly_builder, "typed_configurer"));
	host.set(commandline_handler, "xml_parsing", host.get(xml_parser, "parser"));
	
	host.set(driver, "argc", &argc);
	host.set(driver, "argv", &argv);
	host.set(driver, "assembly_building", host.get(assembly_builder, "go"));
	host.set(driver, "initializations", host.get(commandline_handler, "initializer"));
	
	// Let's run the assembly
	static_cast<Go*>(host.get(driver, "go"))->go();

	MPI_Finalize();

	return EXIT_SUCCESS;
}
