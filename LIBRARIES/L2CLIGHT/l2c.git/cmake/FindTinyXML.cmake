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

#
# Find the tinyxml library and include dir
#
# The following variables are set:
# TinyXML_FOUND        - When false, don't try to use tinyxml
# TinyXML_INCLUDE_DIR  - tinyxml include directory
# TinyXML_INCLUDE_DIRS - All directories to include to use tinyxml (including dependancies)
# TinyXML_LIBRARY      - The tinyxml library
# TinyXML_LIBRARIES    - All libraries to link against to use tinyxml (including dependancies)

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

include(FindPackageHandleStandardArgs)

set(TINYXML_DIR "$ENV{TINYXML_DIR}" CACHE PATH "TinyXML install directory")

# Use pkg-config to get hints about path
if ( PKG_CONFIG_FOUND )
	set(_PKGCONFIG_MODULE "tinyxml")
	if ( DEFINED PACKAGE_FIND_VERSION )
		set(_PKGCONFIG_MODULE "${_PKGCONFIG_MODULE}>=${PACKAGE_FIND_VERSION}")
	endif()
	pkg_check_modules(TINYXML_PKGCONF QUIET "${_PKGCONFIG_MODULE}")
	unset(_PKGCONFIG_ARGS)
endif()

# Include dir
find_path(TinyXML_INCLUDE_DIR
	NAMES tinyxml.h
	PATHS "${TINYXML_DIR}" "$ENV{TINYXML_DIR}" "${TINYXML_DIR}/include" "$ENV{TINYXML_DIR}/include" NO_DEFAULT_PATH
	DOC "tinyxml include directory"
)
find_path(TinyXML_INCLUDE_DIR
	NAMES tinyxml.h
	HINTS ${TINYXML_PKGCONF_INCLUDE_DIRS}
	DOC "tinyxml include directory"
)
list(APPEND TinyXML_INCLUDE_DIRS "${TinyXML_INCLUDE_DIR}")

# Library
find_library(TinyXML_LIBRARY
	NAMES tinyxml
	PATHS "${TINYXML_DIR}" "$ENV{TINYXML_DIR}" "${TINYXML_DIR}/lib" "$ENV{TINYXML_DIR}/lib" NO_DEFAULT_PATH
	DOC "the tinyxml library"
)
find_library(TinyXML_LIBRARY
	NAMES tinyxml
	HINTS ${TINYXML_PKGCONF_LIBRARY_DIRS}
	DOC "the tinyxml library"
)
list(APPEND TinyXML_LIBRARIES "${TinyXML_LIBRARY}")

#TODO: find about version

#Default arguments handling
find_package_handle_standard_args(TinyXML REQUIRED_VARS TinyXML_INCLUDE_DIR TinyXML_LIBRARY)
set(TinyXML_FOUND ${TINYXML_FOUND})

if ( ${TinyXML_FOUND} )
	mark_as_advanced(TinyXML_INCLUDE_DIR TinyXML_LIBRARY)
endif()
