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
# Find the omnithread library and include dir
#
# The following variables are set:
# omnithread3_FOUND        - When false, don't try to use omnithread
# omnithread3_INCLUDE_DIR  - omnithread include directory
# omnithread3_INCLUDE_DIRS - All directories to include to use omnithread (including dependancies)
# omnithread3_LIBRARY      - The omnithread library
# omnithread3_LIBRARIES    - All libraries to link against to use omnithread (including dependancies)

#### Find paths
#
# OMNI_DIR can be used to make it simpler to find the various include
# directories and compiled libraries when omniORB was not installed in the
# usual/well-known directories (e.g. because you made an in tree-source
# compilation or because you installed it in an "unusual" directory).
# Just set OMNI_DIR to point to your specific installation directory.
#
# the OMNI_DIR environment variable (as opposed to the OMNI_DIR cmake variable)
# is also used to search for an omniORB installation.
#
# WARNING: The order of precedence is the following
#    1/ when set OMNI_DIR (the cmake variable)
#    2/ when set OMNI_DIR (the environment variable)
#    3/ the default system path
# This precedence order goes against the usual pratice and default behavior
# of cmake's FIND_* macros. For more on this debate see e.g.
#     http://www.mail-archive.com/kde-buildsystem@kde.org/msg00589.html

#### some libs installed by omniorb
# omnithread    - checked for here: portable thread support
# omniORB4      - depends on us:    base CORBA support
# omniDynamic4  - depends on us:    dynamic features
# COS4          - depends on us:    stubs and skeletons for the COS service interfaces
# COSDynamic4   - depends on us:    dynamic stubs for the COS service interfaces
# omniCodeSets4 - depends on us:    extra code sets for string transformation
# omnisslTP     - depends on us:    SSL transport (built if OpenSSL is available

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

include(FindPackageHandleStandardArgs)

set(OMNI_DIR "$ENV{OMNI_DIR}" CACHE PATH "OmniORB install directory")

# Use pkg-config to get hints about path
if ( PKG_CONFIG_FOUND )
	set(_PKGCONFIG_MODULE "omnithread3")
	if ( DEFINED PACKAGE_FIND_VERSION )
		set(_PKGCONFIG_MODULE "${_PKGCONFIG_MODULE}>=${PACKAGE_FIND_VERSION}")
	endif()
	pkg_check_modules(OMNITHREAD3_PKGCONF QUIET "${_PKGCONFIG_MODULE}")
	unset(_PKGCONFIG_ARGS)
endif()

# Include dir
find_path(omnithread3_INCLUDE_DIR
	NAMES omnithread.h
	PATHS "${OMNI_DIR}" "$ENV{OMNI_DIR}" "${OMNI_DIR}/include" "$ENV{OMNI_DIR}/include" NO_DEFAULT_PATH
	DOC "omnithread include directory"
)
find_path(omnithread3_INCLUDE_DIR
	NAMES omnithread.h
	HINTS ${OMNITHREAD3_PKGCONF_INCLUDE_DIRS}
	DOC "omnithread include directory"
)
list(APPEND omnithread3_INCLUDE_DIRS "${omnithread3_INCLUDE_DIR}")

# Library
find_library(omnithread3_LIBRARY
	NAMES omnithread
	PATHS "${OMNI_DIR}" "$ENV{OMNI_DIR}" "${OMNI_DIR}/lib" "$ENV{OMNI_DIR}/lib" NO_DEFAULT_PATH
	DOC "the omnithread library"
)
find_library(omnithread3_LIBRARY
	NAMES omnithread
	HINTS ${OMNITHREAD3_PKGCONF_LIBRARY_DIRS}
	DOC "the omnithread library"
)
list(APPEND omnithread3_LIBRARIES "${omnithread3_LIBRARY}")

#TODO: find about version

mark_as_advanced(CLEAR OMNI_DIR)

#Default arguments handling
find_package_handle_standard_args(omnithread3 REQUIRED_VARS omnithread3_INCLUDE_DIR omnithread3_LIBRARY)
set(omnithread3_FOUND ${OMNITHREAD3_FOUND})

if ( ${omnithread3_FOUND} )
	mark_as_advanced(FORCE OMNI_DIR omnithread3_INCLUDE_DIR omnithread3_LIBRARY)
endif()
