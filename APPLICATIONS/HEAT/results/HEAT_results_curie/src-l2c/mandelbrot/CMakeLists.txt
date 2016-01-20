project(llcmcpp_example_mandelbrot)
cmake_minimum_required(VERSION 2.8)

include(FindMPI)
include(FindOpenMP)
include(FindPkgConfig)

pkg_check_modules(OMNIORB REQUIRED omniORB4>=4.1)

include_directories(${MPI_INCLUDE_PATH} ${OMNIORB_INCLUDE_DIRS} ${llcmcpp_example_mandelbrot_BINARY_DIR} include)
link_directories(${OMNIORB_LIBRARY_DIRS})

add_custom_command(OUTPUT CorbaMandelbrot.h CorbaMandelbrot.cpp
	COMMAND omniidl -bcxx -Wbkeep_inc_path -Wbh=.h -Wbs=.cpp ${CMAKE_CURRENT_SOURCE_DIR}/idl/CorbaMandelbrot.idl
	DEPENDS idl/CorbaMandelbrot.idl
	VERBATIM
)

add_library(llcm_Dumper MODULE src/Dumper/Dumper.cpp)
install(TARGETS llcm_Dumper LIBRARY DESTINATION lib)

if(${OPENMP_FOUND})
	add_library(llcm_MandelbrotOMP MODULE src/MandelbrotOMP/MandelbrotOMP.cpp)
	set_target_properties(llcm_MandelbrotOMP PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
	install(TARGETS llcm_MandelbrotOMP LIBRARY DESTINATION lib)
else(${OPENMP_FOUND})
	message(WARNING "OpenMP missing, MandelbrotOMP not compiled")
endif(${OPENMP_FOUND})

add_library(llcm_MandelbrotRun MODULE src/MandelbrotRun/MandelbrotRun.cpp)
install(TARGETS llcm_MandelbrotRun LIBRARY DESTINATION lib)

if(${OPENMP_FOUND})
	add_library(llcm_MandelbrotRunMulti MODULE src/MandelbrotRunMulti/MandelbrotRunMulti.cpp)
	set_target_properties(llcm_MandelbrotRunMulti PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
	install(TARGETS llcm_MandelbrotRunMulti LIBRARY DESTINATION lib)
else(${OPENMP_FOUND})
	message(WARNING "OpenMP missing, llcm_MandelbrotRunMulti not compiled")
endif(${OPENMP_FOUND})

add_library(llcm_MandelbrotSeq MODULE src/MandelbrotSeq/MandelbrotSeq.cpp)
install(TARGETS llcm_MandelbrotSeq LIBRARY DESTINATION lib)

if(${MPI_FOUND})
	add_library(llcm_MandelbrotRunMPI MODULE src/MandelbrotRunMPI/MandelbrotRunMPI.cpp)
	target_link_libraries(llcm_MandelbrotRunMPI ${MPI_LIBRARIES})
	foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
		set_target_properties(llcm_MandelbrotRunMPI PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
	endforeach(COMPILE_FLAG)
	foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
		set_target_properties(llcm_MandelbrotRunMPI PROPERTIES LINK_FLAGS ${LINK_FLAG})
	endforeach(LINK_FLAG)
	install(TARGETS llcm_MandelbrotRunMPI LIBRARY DESTINATION lib)
else(${MPI_FOUND})
	message(WARNING "no MPI found, MandelbrotRunMPI will not be compiled")
endif(${MPI_FOUND})

if(${OMNIORB_FOUND})
	add_library(MandelbrotProxyClient MODULE src/MandelbrotProxyClient/mandelbrotproxyclient.cpp CorbaMandelbrot.cpp)
	target_link_libraries(MandelbrotProxyClient ${OMNIORB_LIBRARIES})
	foreach(COMPILE_FLAG IN LISTS OMNIORB_CFLAGS_OTHER)
		set_target_properties(MandelbrotProxyClient PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
	endforeach(COMPILE_FLAG)
	foreach(LINK_FLAG IN LISTS OMNIORB_LDFLAGS_OTHER)
		set_target_properties(MandelbrotProxyClient PROPERTIES LINK_FLAGS ${LINK_FLAG})
	endforeach(LINK_FLAG)
	install(TARGETS MandelbrotProxyClient LIBRARY DESTINATION lib)
else(${OMNIORB_FOUND})
	message(WARNING "CORBA OmniOrb not found, MandelbrotProxyClient will not be compiled")
endif(${OMNIORB_FOUND})

if(${OMNIORB_FOUND})
	add_library(MandelbrotProxyServer MODULE src/MandelbrotProxyServer/mandelbrotproxyserver.cpp CorbaMandelbrot.cpp)
	target_link_libraries(MandelbrotProxyServer ${OMNIORB_LIBRARIES})
	foreach(COMPILE_FLAG IN LISTS OMNIORB_CFLAGS_OTHER)
		set_target_properties(MandelbrotProxyServer PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
	endforeach(COMPILE_FLAG)
	foreach(LINK_FLAG IN LISTS OMNIORB_LDFLAGS_OTHER)
		set_target_properties(MandelbrotProxyServer PROPERTIES LINK_FLAGS ${LINK_FLAG})
	endforeach(LINK_FLAG)
	install(TARGETS MandelbrotProxyServer LIBRARY DESTINATION lib)
else(${OMNIORB_FOUND})
	message(WARNING "CORBA OmniOrb not found, MandelbrotProxyServer will not be compiled")
endif(${OMNIORB_FOUND})

install(DIRECTORY include/ DESTINATION include/mandelbrot FILES_MATCHING PATTERN "*.h" PATTERN "*/*" EXCLUDE)
install(DIRECTORY cad/ DESTINATION lib/llcmcpp3/mandelbrot FILES_MATCHING PATTERN "*.llcmcpp3")
