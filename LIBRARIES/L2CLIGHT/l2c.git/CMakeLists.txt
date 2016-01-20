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

project(LLCMcpp CXX C Fortran)
cmake_minimum_required(VERSION 2.8)

# Use our own modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")


option(L2C   "Enables support for L²C components" ON)
option(MPI   "Enables support for MPI" ON)
option(CORBA "Enables support for CORBA" OFF)
option(GLUON "Enables support for Gluon++ components (requires Charm++)" OFF)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR     lib                      CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR     bin                      CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include                  CACHE PATH "Installation directory for header files")
set(INSTALL_DATA_DIR    share                    CACHE PATH "Installation directory for data files")
set(INSTALL_LAD_DIR     "${INSTALL_LIB_DIR}/lad" CACHE PATH "Installation directory for libraries")

# Make relative paths absolute (needed later on)
foreach(P LIB BIN INCLUDE DATA LAD)
  set(D INSTALL_${P}_DIR)
  if (NOT IS_ABSOLUTE "${${D}}")
    set(${D} "${CMAKE_INSTALL_PREFIX}/${${D}}")
  endif()
endforeach()

find_package(Boost 1.34 COMPONENTS thread REQUIRED)
find_package(Threads REQUIRED)
find_package(TinyXML)
if("${MPI}")
	find_package(MPI REQUIRED)
	include_directories(${MPI_CXX_INCLUDE_PATH})
	add_definitions(-DWITH_MPI)
endif()
if("${CORBA}")
	find_package(OmniORB4 REQUIRED)
	include_directories(${OmniORB4_INCLUDE_DIRS})
	add_definitions(-DWITH_MPI)
endif()

# use inline TinyXML if not available
if(NOT ${TinyXML_FOUND})
	add_library(tinyxml SHARED tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)
	add_definitions(-DTIXML_USE_STL)
	set_property(TARGET tinyxml APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG "DEBUG")
	set(TinyXML_LIBRARIES tinyxml)
	set(TinyXML_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/tinyxml/")
	install(TARGETS tinyxml LIBRARY DESTINATION "${INSTALL_LIB_DIR}")
endif()

include_directories(${TinyXML_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/include ${CMAKE_BINARY_DIR}/include/llcmcpp)
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(base)
if("${L2C}")
	add_subdirectory(l2c)
endif()
if("${MPI}")
	add_subdirectory(mpi)
endif()
if("${CORBA}")
	add_subdirectory(corba)
endif()
if("${MPI}" AND "${CORBA}")
	add_subdirectory(corba_mpi)
endif()
if("${GLUON}")
	add_subdirectory(gluon)
endif()

add_executable(llcmcpp_loader base/src/llcmcpp_loader.cpp)
if("${MPI}")
	set_property(TARGET llcmcpp_loader APPEND PROPERTY COMPILE_FLAGS ${MPI_CXX_COMPILE_FLAGS})
	set_property(TARGET llcmcpp_loader APPEND PROPERTY LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
	target_link_libraries(llcmcpp_loader ${MPI_CXX_LIBRARIES})
endif()
target_link_libraries(llcmcpp_loader InstanceHostLibrary ${CMAKE_THREAD_LIBS_INIT})

configure_file(l2c/src/l2c_loader . @ONLY)

install(TARGETS   llcmcpp_loader RUNTIME         DESTINATION "${INSTALL_BIN_DIR}")
install(PROGRAMS  ${CMAKE_BINARY_DIR}/l2c_loader DESTINATION "${INSTALL_BIN_DIR}")
install(PROGRAMS  l2c/src/fllcmcc.sh             DESTINATION "${INSTALL_BIN_DIR}")
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/   DESTINATION "${INSTALL_INCLUDE_DIR}" FILES_MATCHING PATTERN "*.h")
