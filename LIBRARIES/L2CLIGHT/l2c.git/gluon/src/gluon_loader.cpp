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
#include <sstream>

#include <llcmcpp/go.h>
#include <llcmcpp/host.h>
#include <llcmcpp/initializer.h>
#include <llcmcpp/instancehost.h>

#include <gluon_loader.decl.h>

using ::llcmcpp::Go;
using ::llcmcpp::Host;
using ::llcmcpp::Host2;
using ::llcmcpp::Initializer;
using ::llcmcpp::InstanceHost;
using ::std::cerr;
using ::std::endl;
using ::std::endl;

InstanceHost host;

Host::ComponentInstance gluonprint;

Host::ComponentInstance local_namespace_manager;
Host::ComponentInstance lifecycle_manager;
Host::ComponentInstance namespace_manager;
Host::ComponentInstance collective;
Host::ComponentInstance charm_manager;
Host::ComponentInstance cpp_manager;
Host::ComponentInstance cpp_global_manager;
Host::ComponentInstance type_interpreter;
Host::ComponentInstance charm_buffer;
Host::ComponentInstance charm_array_buffer;
Host::ComponentInstance charm_group_buffer;
Host::ComponentInstance start_buffer;
Host::ComponentInstance cpp_mgt_buffer;
Host::ComponentInstance charm_instantiation_buffer;
Host::ComponentInstance instantiation_buffer;
Host::ComponentInstance assembly_builder;
Host::ComponentInstance module_registrar;
Host::ComponentInstance xml_handler;
Host::ComponentInstance xml_parser;
Host::ComponentInstance commandline_handler;
Host::ComponentInstance driver;

class Runner:
		public CBase_Runner
{
public:
	Runner ( CkArgMsg*  msg )
	{
		CProxy_CharmLoader p = CProxy_CharmLoader::ckNew();
	}
	
};

std::stringbuf mycerr;

class CharmLoader:
		public CBase_CharmLoader
{
public:
	CharmLoader ()
	{
		cerr << mycerr.str() << std::flush;
		static_cast<Go*>(host.get(driver, "go"))->go();
	}
};

void _registerExternalModules(char **argv)
{
	int argc = 0;
	for ( ; argv[argc]; ++argc );
	
	std::streambuf* old_buf = cerr.std::ios::rdbuf(&mycerr);

	gluonprint = host.instantiate("GluonPrint");
	host.print = static_cast<llcmcpp::Print*>(host.get(gluonprint, "print"));
	
	local_namespace_manager = host.instantiate("LocalNamespaceManager");
	lifecycle_manager = host.instantiate("LifecycleManager");
	namespace_manager = host.instantiate("ClusterNamespaceManager");
	collective = host.instantiate("CharmCollective");
	charm_manager = host.instantiate("CharmManager");
	cpp_manager = host.instantiate("CppManager");
	cpp_global_manager = host.instantiate("LocalCppManagementFilter");
	charm_buffer = host.instantiate("RmiManagementBuffer");
	charm_array_buffer = host.instantiate("RmiManagementBuffer");
	charm_group_buffer = host.instantiate("RmiManagementBuffer");
	start_buffer = host.instantiate("StartBuffer");
	cpp_mgt_buffer = host.instantiate("CppManagementBuffer");
	type_interpreter = host.instantiate("TypeInterpreter");
	charm_instantiation_buffer = host.instantiate("CharmInstantiationManagementBuffer");
	instantiation_buffer = host.instantiate("ClusterInstantiationManagementBuffer");
	assembly_builder = host.instantiate("AssemblyBuilder2");
	module_registrar = host.instantiate("ModuleRegistrar");
	xml_handler = host.instantiate("GadHandler");
	xml_parser = host.instantiate("TinyXmlComponent");
	commandline_handler = host.instantiate("CommandlineHandler");
	driver = host.instantiate("GluonDriver");
	
	host.set(local_namespace_manager, "host", static_cast<Host*>(&host));
	
	host.set(lifecycle_manager, "raw_component_management", host.get(local_namespace_manager, "raw_component_manager"));
	host.set(lifecycle_manager, "managed_instances", host.get(local_namespace_manager, "namespace_management"));
	
	host.set(namespace_manager, "local_instantiation_management", host.get(local_namespace_manager, "instantiation_manager"));
	host.set(namespace_manager, "local_rank", host.get(collective, "local_rank"));
	
	host.set(charm_manager, "global_namespace_management", host.get(namespace_manager, "namespace_manager"));
	host.set(charm_manager, "instantiation_management", host.get(namespace_manager, "instantiation_manager"));
	host.set(charm_manager, "local_instantiation_management", host.get(local_namespace_manager, "instantiation_manager"));
	host.set(charm_manager, "local_namespace_management", host.get(local_namespace_manager, "namespace_manager"));
	host.set(charm_manager, "namespace_management", host.get(namespace_manager, "cluster_namespace_manager"));
	host.set(charm_manager, "raw_component_management", host.get(local_namespace_manager, "raw_component_manager"));
	
	host.set(cpp_manager, "args", host.get(commandline_handler, "args"));
	host.set(cpp_manager, "host", static_cast<Host*>(&host));
	host.set(cpp_manager, "raw_component_management", host.get(local_namespace_manager, "raw_component_manager"));
		
	host.set(cpp_global_manager, "local_cpp_management", host.get(cpp_manager, "cpp_manager"));
	host.set(cpp_global_manager, "local_namespace_management", host.get(local_namespace_manager, "namespace_management"));
	host.set(cpp_global_manager, "global_namespace_management", host.get(namespace_manager, "namespace_manager"));
	
	host.set(charm_buffer, "rmi_management",  host.get(charm_manager, "charm_manager"));
	
	host.set(charm_array_buffer, "rmi_management",  host.get(charm_manager, "charm_array_manager"));
	
	host.set(charm_group_buffer, "rmi_management",  host.get(charm_manager, "charm_group_manager"));
	
	host.set(start_buffer, "start_management", host.get(cpp_global_manager, "start_manager"));
	
	host.set(cpp_mgt_buffer, "cpp_management", host.get(cpp_global_manager, "cpp_manager"));
	
	host.set(type_interpreter, "cpp_management2", host.get(cpp_mgt_buffer, "cpp_manager"));
	
	host.set(instantiation_buffer, "instantiation_management", host.get(namespace_manager, "instantiation_manager"));
	
	host.set(charm_instantiation_buffer, "instantiation_management", host.get(charm_manager, "instantiation_manager"));
	
	host.set(assembly_builder, "lifecycle_management", host.get(lifecycle_manager, "lifecycle_manager"));
	host.set(assembly_builder, "executions", host.get(start_buffer, "go"));
	host.set(assembly_builder, "configurations", host.get(charm_buffer, "go"));
	host.set(assembly_builder, "configurations", host.get(charm_array_buffer, "go"));
	host.set(assembly_builder, "configurations", host.get(charm_group_buffer, "go"));
	host.set(assembly_builder, "configurations", host.get(cpp_mgt_buffer, "go"));
	host.set(assembly_builder, "instantiations", host.get(instantiation_buffer, "go"));
	host.set(assembly_builder, "instantiations", host.get(charm_instantiation_buffer, "go"));
	
	host.set(module_registrar, "host", static_cast<Host2*>(&host));
	host.set(module_registrar, "charm_instantiation_management", host.get(charm_instantiation_buffer, "instantiation_manager"));
	host.set(module_registrar, "instantiation_management", host.get(instantiation_buffer, "instantiation_manager"));
	
	host.set(xml_handler, "charm_instantiation_management", host.get(module_registrar, "charm_instantiation_manager"));
	host.set(xml_handler, "instantiation_management", host.get(module_registrar, "instantiation_manager"));
	host.set(xml_handler, "cpp_management", host.get(cpp_mgt_buffer, "cpp_manager"));
	host.set(xml_handler, "start_management", host.get(start_buffer, "start_manager"));
	host.set(xml_handler, "typed_configuration", host.get(type_interpreter, "interpreter"));
	host.set(xml_handler, "charm_management", host.get(charm_buffer, "rmi_manager"));
	host.set(xml_handler, "charm_array_management", host.get(charm_array_buffer, "rmi_manager"));
	host.set(xml_handler, "charm_group_management", host.get(charm_group_buffer, "rmi_manager"));
	
	host.set(xml_parser, "visitor", host.get(xml_handler, "visitor"));
	
	host.set(commandline_handler, "typed_configuration", host.get(type_interpreter, "interpreter"));
	host.set(commandline_handler, "xml_parsing", host.get(xml_parser, "parser"));
	
	host.set(driver, "assembly_builder", host.get(assembly_builder, "assembly_building"));
	host.set(driver, "collective", host.get(collective, "collective"));
	host.set(driver, "initializations", host.get(commandline_handler, "initializer"));
	host.set(driver, "runtime_initializations", host.get(collective, "init"));
	host.set(driver, "runtime_initializations", host.get(charm_manager, "init"));
	
	// Let's run the assembly
	static_cast<Initializer*>(host.get(driver, "init"))->initialize(&argc, &argv);
	
	cerr.std::ios::rdbuf(old_buf);
}

void _createTraces(char **argv)
{
}

#include <gluon_loader.def.h>
