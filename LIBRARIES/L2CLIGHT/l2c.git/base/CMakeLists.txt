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

include_directories(include)

add_library(AssemblyBuilder MODULE src/AssemblyBuilder/assemblybuilder.cpp)
install(TARGETS AssemblyBuilder LIBRARY DESTINATION lib)

add_library(AssemblyBuilder2 MODULE src/AssemblyBuilder2/assemblybuilder2.cpp)
install(TARGETS AssemblyBuilder2 LIBRARY DESTINATION lib)

add_library(Cad3Handler MODULE src/Cad3Handler/cad3handler.cpp)
target_link_libraries(Cad3Handler ${TinyXML_LIBRARIES})
install(TARGETS Cad3Handler LIBRARY DESTINATION lib)

add_library(ClusterInstantiationManagementBuffer MODULE src/ClusterInstantiationManagementBuffer/clusterinstantiationmanagementbuffer.cpp)
install(TARGETS ClusterInstantiationManagementBuffer LIBRARY DESTINATION lib)

add_library(ClusterMpiManagementFilter MODULE src/ClusterMpiManagementFilter/clustermpimanagementfilter.cpp)
install(TARGETS ClusterMpiManagementFilter LIBRARY DESTINATION lib)

add_library(CommandlineHandler MODULE src/CommandlineHandler/commandlinehandler.cpp)
install(TARGETS CommandlineHandler LIBRARY DESTINATION lib)

add_library(CppManagementBuffer MODULE src/CppManagementBuffer/cppmanagementbuffer.cpp)
install(TARGETS CppManagementBuffer LIBRARY DESTINATION lib)

add_library(CppManager MODULE src/CppManager/cppmanager.cpp)
install(TARGETS CppManager LIBRARY DESTINATION lib)

add_library(DistributedInstantiationManagementBuffer MODULE src/DistributedInstantiationManagementBuffer/distributedinstantiationmanagementbuffer.cpp)
install(TARGETS DistributedInstantiationManagementBuffer LIBRARY DESTINATION lib)

add_library(DistributedNamespaceManager MODULE src/DistributedNamespaceManager/distributednamespacemanager.cpp)
install(TARGETS DistributedNamespaceManager LIBRARY DESTINATION lib)

add_library(Driver MODULE src/Driver/driver.cpp)
install(TARGETS Driver LIBRARY DESTINATION lib)

add_library(Driver2 MODULE src/Driver2/driver2.cpp)
install(TARGETS Driver2 LIBRARY DESTINATION lib)

add_library(GridInstantiationManagementBuffer MODULE src/GridInstantiationManagementBuffer/gridinstantiationmanagementbuffer.cpp)
install(TARGETS GridInstantiationManagementBuffer LIBRARY DESTINATION lib)

add_library(InstanceHost MODULE src/InstanceHost/instancehost.cpp)
target_link_libraries(InstanceHost ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS InstanceHost LIBRARY DESTINATION lib)

add_library(InstanceHostLibrary SHARED src/InstanceHost/instancehost.cpp)
target_link_libraries(InstanceHostLibrary ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS InstanceHostLibrary LIBRARY DESTINATION lib)

add_library(LifecycleManager MODULE src/LifecycleManager/lifecyclemanager.cpp)
install(TARGETS LifecycleManager LIBRARY DESTINATION lib)

add_library(LocalCppManagementFilter MODULE src/LocalCppManagementFilter/localcppmanagementfilter.cpp)
install(TARGETS LocalCppManagementFilter LIBRARY DESTINATION lib)

add_library(LocalInstantiationFilter MODULE src/LocalInstantiationFilter/localinstantiationfilter.cpp)
install(TARGETS LocalInstantiationFilter LIBRARY DESTINATION lib)

add_library(LocalInstantiationManagementBuffer MODULE src/LocalInstantiationManagementBuffer/localinstantiationmanagementbuffer.cpp)
install(TARGETS LocalInstantiationManagementBuffer LIBRARY DESTINATION lib)

add_library(LocalNamespaceManager MODULE src/LocalNamespaceManager/localnamespacemanager.cpp)
install(TARGETS LocalNamespaceManager LIBRARY DESTINATION lib)

add_library(MpiManagementBuffer MODULE src/MpiManagementBuffer/mpimanagementbuffer.cpp)
install(TARGETS MpiManagementBuffer LIBRARY DESTINATION lib)

add_library(RmiManagementBuffer MODULE src/RmiManagementBuffer/rmimanagementbuffer.cpp)
install(TARGETS RmiManagementBuffer LIBRARY DESTINATION lib)

add_library(StartBuffer MODULE src/StartBuffer/startbuffer.cpp)
install(TARGETS StartBuffer LIBRARY DESTINATION lib)

add_library(TinyXmlComponent MODULE src/TinyXmlComponent/tinyxmlcomponent.cpp)
target_link_libraries(TinyXmlComponent ${TinyXML_LIBRARIES})
install(TARGETS TinyXmlComponent LIBRARY DESTINATION lib)

add_library(TypeInterpreter MODULE src/TypeInterpreter/typeinterpreter.cpp)
install(TARGETS TypeInterpreter LIBRARY DESTINATION lib)

add_library(TypeSetterBuffer MODULE src/TypeSetterBuffer/typesetterbuffer.cpp)
install(TARGETS TypeSetterBuffer LIBRARY DESTINATION lib)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include/llcmcpp")
file(COPY include/ DESTINATION "${CMAKE_BINARY_DIR}/include/llcmcpp/" PATTERN "*.h" PATTERN "*.f")
