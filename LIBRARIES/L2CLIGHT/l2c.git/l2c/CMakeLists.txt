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

# LadHandler
add_library(LadHandler MODULE src/LadHandler/ladhandler.cpp)
target_link_libraries(LadHandler ${TinyXML_LIBRARIES})
install(TARGETS LadHandler LIBRARY DESTINATION lib)

# L2CPrint
add_library(L2CPrint MODULE src/L2CPrint/l2cprint.cpp)
install(TARGETS L2CPrint LIBRARY DESTINATION lib)


install(DIRECTORY lad/ DESTINATION "${INSTALL_LAD_DIR}/l2c" FILES_MATCHING PATTERN "*.lad")
install(DIRECTORY lad/ DESTINATION "${INSTALL_DATA_DIR}"    FILES_MATCHING PATTERN "*.lad.sh")
install(DIRECTORY xsd/ DESTINATION "${INSTALL_DATA_DIR}"    FILES_MATCHING PATTERN "*.xsd")
