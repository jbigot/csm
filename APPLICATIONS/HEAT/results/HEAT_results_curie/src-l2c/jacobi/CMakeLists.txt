project(llcmcpp_components)
cmake_minimum_required(VERSION 2.8)

include(FindOpenMP)
include(FindMPI)
find_package(Boost 1.42 COMPONENTS thread system)
find_package(Threads)

if(NOT ${OPENMP_FOUND})
	message(SEND_ERROR "no OpenMP found, jacobi can not be compiled")
endif(NOT ${OPENMP_FOUND})

if(NOT ${MPI_FOUND})
	message(SEND_ERROR "no MPI found, jacobi can not be compiled")
endif(NOT ${MPI_FOUND})

if(NOT ${Boost_FOUND})
	message(SEND_ERROR "no Boost found, jacobi can not be compiled")
endif(NOT ${Boost_FOUND})

include_directories(include ${MPI_INCLUDE_PATH} ${Boost_INCLUDE_DIRS} ../threads/include)
link_directories(${Boost_LIBRARY_DIRS})

# Components

add_library(DataInitializer MODULE src/DataInitializer/datainitializer.cpp)
install(TARGETS DataInitializer LIBRARY DESTINATION lib)

add_library(Jacobi4Connectors MODULE src/Jacobi4Connectors/jacobi4connectors.cpp)
target_link_libraries(Jacobi4Connectors ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(Jacobi4Connectors PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(Jacobi4Connectors PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS Jacobi4Connectors LIBRARY DESTINATION lib)

add_library(JacocpuCore MODULE src/JacocpuCore/jacocpucore.cpp)
install(TARGETS JacocpuCore LIBRARY DESTINATION lib)

add_library(JacobiCore MODULE src/JacobiCore/jacobicore.cpp)
install(TARGETS JacobiCore LIBRARY DESTINATION lib)

add_library(JacobiCoreNiter MODULE src/JacobiCoreNiter/jacobicoreniter.cpp)
install(TARGETS JacobiCoreNiter LIBRARY DESTINATION lib)

add_library(JacobiOMP MODULE src/JacobiOMP/jacobiomp.cpp)
set_target_properties(JacobiOMP PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
install(TARGETS JacobiOMP LIBRARY DESTINATION lib)

add_library(JacobiSeq MODULE src/JacobiSeq/jacobiseq.cpp)
install(TARGETS JacobiSeq LIBRARY DESTINATION lib)

add_library(MAlloc MODULE src/MAlloc/malloc.cpp)
install(TARGETS MAlloc LIBRARY DESTINATION lib)

add_library(MpiConcentrator MODULE src/MpiConcentrator/mpiconcentrator.cpp)
target_link_libraries(MpiConcentrator ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(MpiConcentrator PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(MpiConcentrator PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS MpiConcentrator LIBRARY DESTINATION lib)

add_library(MPIConnector MODULE src/MPIConnector/mpiconnector.cpp)
target_link_libraries(MPIConnector ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(MPIConnector PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(MPIConnector PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS MPIConnector LIBRARY DESTINATION lib)

add_library(MPIDriver MODULE src/MPIDriver/mpidriver.cpp)
target_link_libraries(MPIDriver ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(MPIDriver PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(MPIDriver PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS MPIDriver LIBRARY DESTINATION lib)

add_library(MPIDriverOverlap MODULE src/MPIDriverOverlap/mpidriveroverlap.cpp)
target_link_libraries(MPIDriverOverlap ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(MPIDriverOverlap PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(MPIDriverOverlap PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS MPIDriverOverlap LIBRARY DESTINATION lib)

add_library(PthreadConnector MODULE src/PthreadConnector/pthreadconnector.cpp)
target_link_libraries(PthreadConnector ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS PthreadConnector LIBRARY DESTINATION lib)

add_library(PthreadConnectorDual MODULE src/PthreadConnectorDual/pthreadconnectordual.cpp)
target_link_libraries(PthreadConnectorDual ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS PthreadConnectorDual LIBRARY DESTINATION lib)

add_library(PthreadDriver MODULE src/PthreadDriver/pthreaddriver.cpp)
target_link_libraries(PthreadDriver ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS PthreadDriver LIBRARY DESTINATION lib)

add_library(SeqDriver MODULE src/SeqDriver/seqdriver.cpp)
install(TARGETS SeqDriver LIBRARY DESTINATION lib)

add_library(SimpleXpDriver MODULE src/SimpleXpDriver/simplexpdriver.cpp)
install(TARGETS SimpleXpDriver LIBRARY DESTINATION lib)

add_library(ThreadDriver MODULE src/ThreadDriver/threaddriver.cpp)
target_link_libraries(ThreadDriver ${Boost_LIBRARIES})
install(TARGETS ThreadDriver LIBRARY DESTINATION lib)

add_library(ThreadXp MODULE src/ThreadXp/threadxp.cpp)
target_link_libraries(ThreadXp ${Boost_LIBRARIES})
install(TARGETS ThreadXp LIBRARY DESTINATION lib)

add_library(XP MODULE src/XP/xp.cpp)
install(TARGETS XP LIBRARY DESTINATION lib)

add_library(XPDriver MODULE src/XPDriver/xpdriver.cpp)
install(TARGETS XPDriver LIBRARY DESTINATION lib)

add_library(XPValidate MODULE src/XPValidate/xpvalidate.cpp)
install(TARGETS XPValidate LIBRARY DESTINATION lib)

# Non components

add_executable(jacobi src/jacobi.cpp)
install(TARGETS jacobi RUNTIME DESTINATION bin)

add_executable(jacobi_omp src/jacobi_omp.cpp)
set_target_properties(jacobi_omp PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} LINK_FLAGS ${OpenMP_CXX_FLAGS})
install(TARGETS jacobi_omp RUNTIME DESTINATION bin)

add_executable(jacobi_mpi src/jacobi_mpi.cpp)
target_link_libraries(jacobi_mpi ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(jacobi_mpi PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(jacobi_mpi PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS jacobi_mpi RUNTIME DESTINATION bin)

add_executable(jacobi_mpi_overlap src/jacobi_mpi_overlap.cpp)
target_link_libraries(jacobi_mpi_overlap ${MPI_LIBRARIES})
foreach(COMPILE_FLAG IN LISTS MPI_COMPILE_FLAGS)
	set_target_properties(jacobi_mpi_overlap PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG})
endforeach(COMPILE_FLAG)
foreach(LINK_FLAG IN LISTS MPI_LINK_FLAGS)
	set_target_properties(jacobi_mpi_overlap PROPERTIES LINK_FLAGS ${LINK_FLAG})
endforeach(LINK_FLAG)
install(TARGETS jacobi_mpi_overlap RUNTIME DESTINATION bin)

add_executable(jacobi_thread src/jacobi_thread.cpp)
target_link_libraries(jacobi_thread ${Boost_LIBRARIES})
install(TARGETS jacobi_thread RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include/jacobi FILES_MATCHING PATTERN "*.h" PATTERN "*/*" EXCLUDE)
install(DIRECTORY cad/ DESTINATION lib/llcmcpp3/jacobi FILES_MATCHING PATTERN "*.llcmcpp3")
install(DIRECTORY cad/ DESTINATION lib/llcmcpp3/jacobi FILES_MATCHING PATTERN "*.llcmcpp3.sh")
