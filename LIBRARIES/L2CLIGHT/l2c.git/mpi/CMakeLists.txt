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


add_library(ClusterNamespaceManager MODULE src/ClusterNamespaceManager/clusternamespacemanager.cpp)
target_link_libraries(ClusterNamespaceManager ${MPI_CXX_LIBRARIES})
set_property(TARGET ClusterNamespaceManager APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
set_property(TARGET ClusterNamespaceManager APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
install(TARGETS ClusterNamespaceManager LIBRARY DESTINATION lib)

add_library(MpiBarrier MODULE src/MpiBarrier/mpibarrier.cpp)
target_link_libraries(MpiBarrier ${MPI_CXX_LIBRARIES})
set_property(TARGET MpiBarrier APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
set_property(TARGET MpiBarrier APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
install(TARGETS MpiBarrier LIBRARY DESTINATION lib)

add_library(MpiInitializer MODULE src/MpiInitializer/mpiinitializer.cpp)
target_link_libraries(MpiInitializer ${MPI_CXX_LIBRARIES})
set_property(TARGET MpiInitializer APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
set_property(TARGET MpiInitializer APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
install(TARGETS MpiInitializer LIBRARY DESTINATION lib)

add_library(MpiIorBroadcastRoot MODULE src/MpiIorBroadcastRoot/mpiiorbroadcastroot.cpp)
target_link_libraries(MpiIorBroadcastRoot ${MPI_CXX_LIBRARIES})
set_property(TARGET MpiIorBroadcastRoot APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
set_property(TARGET MpiIorBroadcastRoot APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
install(TARGETS MpiIorBroadcastRoot LIBRARY DESTINATION lib)

add_library(MpiIorBroadcastSlave MODULE src/MpiIorBroadcastSlave/mpiiorbroadcastslave.cpp)
target_link_libraries(MpiIorBroadcastSlave ${MPI_CXX_LIBRARIES})
set_property(TARGET MpiIorBroadcastSlave APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
set_property(TARGET MpiIorBroadcastSlave APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
install(TARGETS MpiIorBroadcastSlave LIBRARY DESTINATION lib)

add_library(MpiManager MODULE src/MpiManager/mpimanager.cpp)
target_link_libraries(MpiManager ${MPI_CXX_LIBRARIES})
set_property(TARGET MpiManager APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
set_property(TARGET MpiManager APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
install(TARGETS MpiManager LIBRARY DESTINATION lib)
