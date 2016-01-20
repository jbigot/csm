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

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/interbridge.h ${CMAKE_BINARY_DIR}/interbridge.cpp
	COMMAND ${OmniORB4_OMNIIDL} -bcxx -Wbkeep_inc_path -Wbh=.h -Wbs=.cpp ${CMAKE_CURRENT_SOURCE_DIR}/idl/interbridge.idl
	DEPENDS idl/interbridge.idl
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	VERBATIM
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/master.h ${CMAKE_BINARY_DIR}/master.cpp
	COMMAND ${OmniORB4_OMNIIDL} -bcxx -Wbkeep_inc_path -Wbh=.h -Wbs=.cpp ${CMAKE_CURRENT_SOURCE_DIR}/idl/master.idl
	DEPENDS idl/master.idl ${CMAKE_BINARY_DIR}/interbridge.h ${CMAKE_BINARY_DIR}/slave.h
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	VERBATIM
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/slave.h ${CMAKE_BINARY_DIR}/slave.cpp
	COMMAND ${OmniORB4_OMNIIDL} -bcxx -Wbkeep_inc_path -Wbh=.h -Wbs=.cpp ${CMAKE_CURRENT_SOURCE_DIR}/idl/slave.idl
	DEPENDS idl/slave.idl ${CMAKE_BINARY_DIR}/interbridge.h
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	VERBATIM
)

add_library(CorbaBridgeMaster MODULE src/CorbaBridgeMaster/corbabridgemaster.cpp ${CMAKE_BINARY_DIR}/interbridge.cpp ${CMAKE_BINARY_DIR}/master.cpp ${CMAKE_BINARY_DIR}/slave.cpp)
target_link_libraries(CorbaBridgeMaster ${OmniORB4_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS CorbaBridgeMaster LIBRARY DESTINATION lib)

add_library(CorbaBridgeSlave MODULE src/CorbaBridgeSlave/corbabridgeslave.cpp ${CMAKE_BINARY_DIR}/interbridge.cpp ${CMAKE_BINARY_DIR}/master.cpp ${CMAKE_BINARY_DIR}/slave.cpp)
target_link_libraries(CorbaBridgeSlave ${OmniORB4_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS CorbaBridgeSlave LIBRARY DESTINATION lib)

add_library(CorbaManager MODULE src/CorbaManager/corbamanager.cpp)
target_link_libraries(CorbaManager ${OmniORB4_LIBRARIES})
install(TARGETS CorbaManager LIBRARY DESTINATION lib)
