#!/bin/sh
#     This file is part of LLCMc++ (a.k.a. llcmcpp)
#     Copyright (C) 2011 INRIA Julien Bigot <julien.bigot@inria.fr>
#     Copyright (C) 2011 INRIA Christian Pérez <christian.perez@inria.fr>
# 
#     LLCMc++ is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     LLCMc++ is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Lesser General Public License for more details.
# 
#     You should have received a copy of the GNU Lesser General Public License
#     along with LLCMc++.  If not, see <http://www.gnu.org/licenses/>.

part1()
{
cat << EOF
<?xml version='1.0'?>
<!--
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
-->
<lad xmlns="http://www.inria.fr/graal/lad/1.0.0">
	<mpi>
		<process>
			<start_property instance="driver_0" property="go"/>
			
			<instance id="local_namespace_manager_0" type="LocalNamespaceManager" >
				<property id="host3"><systemref name="host3"/></property>
			</instance>
			
			<instance id="mpi_bridge_0" type="MpiBarrier" />
			
			<instance id="corba_bridge_0" type="CorbaBridgeMaster">
				<property id="local_host"><cppref instance="grid_namespace_manager_0" property="local_host"/></property>
				<property id="raw_component_management"><cppref instance="local_namespace_manager_0" property="raw_component_manager"/></property>
			</instance>
		
			<instance id="corba_config_0" type="MpiIorBroadcastRoot">
				<property id="ior"><cppref instance="corba_bridge_0" property="master_ior"/></property>
			</instance>
			
			<instance id="lifecycle_manager_0" type="LifecycleManager" >
				<property id="raw_component_management"><cppref instance="local_namespace_manager_0" property="raw_component_manager"/></property>
				<property id="managed_instances"><cppref instance="local_namespace_manager_0" property="namespace_management"/></property>
			</instance>
			
			<instance id="cluster_namespace_manager_0" type="ClusterNamespaceManager" >
				<property id="local_instantiation_management"><cppref instance="local_namespace_manager_0" property="instantiation_manager"/></property>
				<property id="local_rank"><cppref instance="mpi_manager_0" property="local_rank"/></property>
			</instance>
			
			<instance id="grid_namespace_manager_0" type="GridNamespaceManager" >
				<property id="cluster_instantiation_management"><cppref instance="cluster_namespace_manager_0" property="instantiation_manager"/></property>
				<property id="local_rank"><cppref instance="mpi_manager_0" property="local_rank"/></property>
			</instance>
			
			<instance id="cpp_manager_0" type="CppManager" >
				<property id="raw_component_management"><cppref instance="local_namespace_manager_0" property="raw_component_manager"/></property>
				<property id="args"><cppref instance="commandline_handler_0" property="args"/></property>
				<property id="host"><systemref name="host"/></property>
				<property id="host3"><systemref name="host3"/></property>
			</instance>
			
			<instance id="cpp_global_manager_0" type="LocalCppManagementFilter" >
				<property id="local_cpp_management"><cppref instance="cpp_manager_0" property="cpp_manager"/></property>
				<property id="local_namespace_management"><cppref instance="local_namespace_manager_0" property="namespace_management"/></property>
				<property id="global_namespace_management"><cppref instance="grid_namespace_manager_0" property="namespace_manager"/></property>
			</instance>
			
			<instance id="mpi_manager_0" type="MpiManager" >
				<property id="cluster_namespace_management"><cppref instance="cluster_namespace_manager_0" property="cluster_namespace_manager"/></property>
				<property id="raw_component_management"><cppref instance="local_namespace_manager_0" property="raw_component_manager"/></property>
			</instance>
			
			<instance id="mpi_global_manager_0" type="ClusterMpiManagementFilter" >
				<property id="cluster_namespace_management"><cppref instance="cluster_namespace_manager_0" property="namespace_manager"/></property>
				<property id="grid_namespace_management"><cppref instance="grid_namespace_manager_0" property="namespace_manager"/></property>
				<property id="mpi_management"><cppref instance="mpi_manager_0" property="mpi_manager"/></property>
			</instance>
			
			<instance id="corba_manager_0" type="CorbaManager" >
				<property id="corba_access"><cppref instance="corba_bridge_0" property="corba"/></property>
				<property id="global_namespace_management"><cppref instance="grid_namespace_manager_0" property="namespace_manager"/></property>
				<property id="local_namespace_management"><cppref instance="local_namespace_manager_0" property="namespace_management"/></property>
				<property id="namespace_management"><cppref instance="grid_namespace_manager_0" property="distributed_placement_manager"/></property>
				<property id="raw_component_management"><cppref instance="local_namespace_manager_0" property="raw_component_manager"/></property>
			</instance>
			
			<instance id="type_interpreter_0" type="TypeInterpreter" >
				<property id="cpp_management"><cppref instance="cpp_global_manager_0" property="cpp_manager"/></property>
			</instance>
			
			<instance id="assembly_builder_0" type="AssemblyBuilder" >
				<property id="collective_access"><cppref instance="corba_bridge_0" property="collective"/></property>
				<property id="corba_management"><cppref instance="corba_manager_0" property="corba_manager"/></property>
				<property id="cpp_management"><cppref instance="cpp_global_manager_0" property="cpp_manager"/></property>
				<property id="grid_instantiation_management"><cppref instance="grid_namespace_manager_0" property="instantiation_manager"/></property>
				<property id="lifecycle_management"><cppref instance="lifecycle_manager_0" property="lifecycle_manager"/></property>
				<property id="mpi_management"><cppref instance="mpi_global_manager_0" property="mpi_manager"/></property>
				<property id="typed_configuration"><cppref instance="type_interpreter_0" property="interpreter"/></property>
			</instance>
			
			<instance id="xml_handler_0" type="LadHandler" >
				<property id="corba_management"><cppref instance="assembly_builder_0" property="corba_manager"/></property>
				<property id="cpp_management"><cppref instance="assembly_builder_0" property="cpp_manager"/></property>
				<property id="grid_instantiation_management"><cppref instance="assembly_builder_0" property="grid_instantiation_manager"/></property>
				<property id="mpi_management"><cppref instance="assembly_builder_0" property="mpi_manager"/></property>
				<property id="typed_configuration"><cppref instance="assembly_builder_0" property="typed_configurer"/></property>
			</instance>
			
			<instance id="xml_parser_0" type="TinyXmlComponent" >
				<property id="visitor"><cppref instance="xml_handler_0" property="visitor"/></property>
			</instance>
			
			<instance id="commandline_handler_0" type="CommandlineHandler" >
				<property id="typed_configuration"><cppref instance="assembly_builder_0" property="typed_configurer"/></property>
				<property id="xml_parsing"><cppref instance="xml_parser_0" property="parser"/></property>
			</instance>
			
			<instance id="driver_0" type="Driver2" >
				<property id="initializations">
					<cppref instance="commandline_handler_0" property="initializer"/>
					<cppref instance="mpi_manager_0" property="initializer"/>
					<cppref instance="grid_namespace_manager_0" property="initializer"/>
					<cppref instance="corba_bridge_0" property="initializer"/>
					<cppref instance="corba_config_0" property="initializer"/>
					<cppref instance="corba_bridge_0" property="initializer2"/>
				</property>
				<property id="assembly_building"><cppref instance="assembly_builder_0" property="go"/></property>
				<property id="argc"><systemref name="argc"/></property>
				<property id="argv"><systemref name="argv"/></property>
			</instance>
		</process>
		
EOF
}

part2()
{
NBPROCESS="$1"
echo NBPROCESS = "$NBPROCESS" 1>&2
N=1
while [ "$N" -lt "$NBPROCESS" ]
do
cat << EOF
		<process>
			<start_property instance="driver_$N" property="go"/>
			
			<instance id="local_namespace_manager_$N" type="LocalNamespaceManager" >
				<property id="host3"><systemref name="host3"/></property>
			</instance>
			
			<instance id="mpi_bridge_$N" type="MpiBarrier" />
			
			<instance id="corba_bridge_$N" type="CorbaBridgeSlave">
				<property id="local_host"><cppref instance="grid_namespace_manager_$N" property="local_host"/></property>
				<property id="raw_component_management"><cppref instance="local_namespace_manager_$N" property="raw_component_manager"/></property>
			</instance>
		
			<instance id="corba_config_$N" type="MpiIorBroadcastSlave">
				<property id="ior"><cppref instance="corba_bridge_$N" property="master_ior"/></property>
			</instance>
			
			<instance id="lifecycle_manager_$N" type="LifecycleManager" >
				<property id="raw_component_management"><cppref instance="local_namespace_manager_$N" property="raw_component_manager"/></property>
				<property id="managed_instances"><cppref instance="local_namespace_manager_$N" property="namespace_management"/></property>
			</instance>
			
			<instance id="cluster_namespace_manager_$N" type="ClusterNamespaceManager" >
				<property id="local_instantiation_management"><cppref instance="local_namespace_manager_$N" property="instantiation_manager"/></property>
				<property id="local_rank"><cppref instance="mpi_manager_$N" property="local_rank"/></property>
			</instance>
			
			<instance id="grid_namespace_manager_$N" type="GridNamespaceManager" >
				<property id="cluster_instantiation_management"><cppref instance="cluster_namespace_manager_$N" property="instantiation_manager"/></property>
				<property id="local_rank"><cppref instance="mpi_manager_$N" property="local_rank"/></property>
			</instance>
			
			<instance id="cpp_manager_$N" type="CppManager" >
				<property id="raw_component_management"><cppref instance="local_namespace_manager_$N" property="raw_component_manager"/></property>
				<property id="args"><cppref instance="commandline_handler_$N" property="args"/></property>
				<property id="host"><systemref name="host"/></property>
				<property id="host3"><systemref name="host3"/></property>
			</instance>
			
			<instance id="cpp_global_manager_$N" type="LocalCppManagementFilter" >
				<property id="local_cpp_management"><cppref instance="cpp_manager_$N" property="cpp_manager"/></property>
				<property id="local_namespace_management"><cppref instance="local_namespace_manager_$N" property="namespace_management"/></property>
				<property id="global_namespace_management"><cppref instance="grid_namespace_manager_$N" property="namespace_manager"/></property>
			</instance>
			
			<instance id="mpi_manager_$N" type="MpiManager" >
				<property id="cluster_namespace_management"><cppref instance="cluster_namespace_manager_$N" property="cluster_namespace_manager"/></property>
				<property id="raw_component_management"><cppref instance="local_namespace_manager_$N" property="raw_component_manager"/></property>
			</instance>
			
			<instance id="mpi_global_manager_$N" type="ClusterMpiManagementFilter" >
				<property id="cluster_namespace_management"><cppref instance="cluster_namespace_manager_$N" property="namespace_manager"/></property>
				<property id="grid_namespace_management"><cppref instance="grid_namespace_manager_$N" property="namespace_manager"/></property>
				<property id="mpi_management"><cppref instance="mpi_manager_$N" property="mpi_manager"/></property>
			</instance>
			
			<instance id="corba_manager_$N" type="CorbaManager" >
				<property id="corba_access"><cppref instance="corba_bridge_$N" property="corba"/></property>
				<property id="global_namespace_management"><cppref instance="grid_namespace_manager_$N" property="namespace_manager"/></property>
				<property id="local_namespace_management"><cppref instance="local_namespace_manager_$N" property="namespace_management"/></property>
				<property id="namespace_management"><cppref instance="grid_namespace_manager_$N" property="distributed_placement_manager"/></property>
				<property id="raw_component_management"><cppref instance="local_namespace_manager_$N" property="raw_component_manager"/></property>
			</instance>
			
			<instance id="type_interpreter_$N" type="TypeInterpreter" >
				<property id="cpp_management"><cppref instance="cpp_global_manager_$N" property="cpp_manager"/></property>
			</instance>
			
			<instance id="assembly_builder_$N" type="AssemblyBuilder" >
				<property id="collective_access"><cppref instance="corba_bridge_$N" property="collective"/></property>
				<property id="corba_management"><cppref instance="corba_manager_$N" property="corba_manager"/></property>
				<property id="cpp_management"><cppref instance="cpp_global_manager_$N" property="cpp_manager"/></property>
				<property id="grid_instantiation_management"><cppref instance="grid_namespace_manager_$N" property="instantiation_manager"/></property>
				<property id="lifecycle_management"><cppref instance="lifecycle_manager_$N" property="lifecycle_manager"/></property>
				<property id="mpi_management"><cppref instance="mpi_global_manager_$N" property="mpi_manager"/></property>
				<property id="typed_configuration"><cppref instance="type_interpreter_$N" property="interpreter"/></property>
			</instance>
			
			<instance id="xml_handler_$N" type="LadHandler" >
				<property id="corba_management"><cppref instance="assembly_builder_$N" property="corba_manager"/></property>
				<property id="cpp_management"><cppref instance="assembly_builder_$N" property="cpp_manager"/></property>
				<property id="grid_instantiation_management"><cppref instance="assembly_builder_$N" property="grid_instantiation_manager"/></property>
				<property id="mpi_management"><cppref instance="assembly_builder_$N" property="mpi_manager"/></property>
				<property id="typed_configuration"><cppref instance="assembly_builder_$N" property="typed_configurer"/></property>
			</instance>
			
			<instance id="xml_parser_$N" type="TinyXmlComponent" >
				<property id="visitor"><cppref instance="xml_handler_$N" property="visitor"/></property>
			</instance>
			
			<instance id="commandline_handler_$N" type="CommandlineHandler" >
				<property id="typed_configuration"><cppref instance="assembly_builder_$N" property="typed_configurer"/></property>
				<property id="xml_parsing"><cppref instance="xml_parser_$N" property="parser"/></property>
			</instance>
			
			<instance id="driver_$N" type="Driver2" >
				<property id="initializations">
					<cppref instance="commandline_handler_$N" property="initializer"/>
					<cppref instance="mpi_manager_$N" property="initializer"/>
					<cppref instance="grid_namespace_manager_$N" property="initializer"/>
					<cppref instance="corba_config_$N" property="initializer"/>
					<cppref instance="corba_bridge_$N" property="initializer"/>
					<!-- <cppref instance="corba_bridge_$N" property="initializer2"/> -->
				</property>
				<property id="assembly_building"><cppref instance="assembly_builder_$N" property="go"/></property>
				<property id="argc"><systemref name="argc"/></property>
				<property id="argv"><systemref name="argv"/></property>
			</instance>
		</process>
		
EOF
N=`expr "$N" + 1`; done
}

part3()
{
cat << EOF
		<communicator>
			<peer instance="mpi_bridge_0" property="comm"/>
EOF
}

part4()
{
NBPROCESS="$1"
N=1
while [ "$N" -lt "$NBPROCESS" ]
do
cat << EOF
			<peer instance="mpi_bridge_$N" property="comm"/>
EOF
N=`expr "$N" + 1`; done
}

part5()
{
cat << EOF
		</communicator>
		
		<communicator>
			<peer instance="mpi_manager_0" property="comm"/>
EOF
}

part6()
{
NBPROCESS="$1"
N=1
while [ "$N" -lt "$NBPROCESS" ]
do
cat << EOF
			<peer instance="mpi_manager_$N" property="comm"/>
EOF
N=`expr "$N" + 1`; done
}

part7()
{
cat << EOF
		</communicator>
		
		<communicator>
			<peer instance="corba_config_0" property="comm"/>
EOF
}

part8()
{
NBPROCESS="$1"
N=1
while [ "$N" -lt "$NBPROCESS" ]
do
cat << EOF
			<peer instance="corba_config_$N" property="comm"/>
EOF
N=`expr "$N" + 1`; done
}

part9()
{
cat << EOF
		</communicator>
	</mpi>
</lad>
EOF
}

NBPROCESS="$1"
part1
part2 "$NBPROCESS"
part3
part4 "$NBPROCESS"
part5
part6 "$NBPROCESS"
part7
part8 "$NBPROCESS"
part9
