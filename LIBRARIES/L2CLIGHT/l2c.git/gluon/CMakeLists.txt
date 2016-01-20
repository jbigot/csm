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

find_package(Charm REQUIRED)

include_directories(include ${Charm_INCLUDE_DIRS})
link_directories(${gluon_BINARY_DIR})

ci_files(CharmCollective_HEADERS src/CharmCollective/CharmCollective.ci)
add_library(CharmCollective MODULE src/CharmCollective/charmcollective.cpp ${CharmCollective_HEADERS})
target_link_libraries(CharmCollective ${Charm_LIBRARIES})
set_property(TARGET CharmCollective APPEND PROPERTY COMPILE_FLAGS ${Charm_COMPILE_FLAGS})
install(TARGETS CharmCollective LIBRARY DESTINATION lib)

add_library(CharmInstantiationManagementBuffer MODULE src/CharmInstantiationManagementBuffer/charminstantiationmanagementbuffer.cpp)
install(TARGETS CharmInstantiationManagementBuffer LIBRARY DESTINATION lib)

ci_files(CharmManager_HEADERS src/CharmManager/charmmanager.ci)
add_library(CharmManager MODULE src/CharmManager/charmmanager.cpp ${CharmManager_HEADERS})
target_link_libraries(CharmManager ${Charm_LIBRARIES})
set_property(TARGET CharmManager APPEND PROPERTY COMPILE_FLAGS ${Charm_COMPILE_FLAGS})
install(TARGETS CharmManager LIBRARY DESTINATION lib)

add_library(GadHandler MODULE src/GadHandler/gadhandler.cpp)
target_link_libraries(GadHandler ${TinyXML_LIBRARIES})
install(TARGETS GadHandler LIBRARY DESTINATION lib)

add_library(GluonDriver MODULE src/GluonDriver/gluondriver.cpp)
install(TARGETS GluonDriver LIBRARY DESTINATION lib)

add_library(ModuleRegistrar MODULE src/ModuleRegistrar/moduleregistrar.cpp)
target_link_libraries(ModuleRegistrar ${CMAKE_DL_LIBS})
install(TARGETS ModuleRegistrar LIBRARY DESTINATION lib)

add_library(GluonPrint MODULE src/GluonPrint/gluonprint.cpp)
target_link_libraries(GluonPrint ${CMAKE_DL_LIBS})
install(TARGETS GluonPrint LIBRARY DESTINATION lib)

ci_files(gluon_loader_HEADERS src/gluon_loader.ci)
add_charm_executable(gluon_loader src/gluon_loader.cpp ${gluon_loader_HEADERS})
target_link_libraries(gluon_loader -lInstanceHostLibrary)
install(TARGETS gluon_loader RUNTIME DESTINATION bin)

install(DIRECTORY ${gluon_SOURCE_DIR}/include/ DESTINATION "${INSTALL_INCLUDE_DIR}/gluon" FILES_MATCHING PATTERN "*.h")
