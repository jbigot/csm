project(llcmcpp_components)
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(llcm_BasicBC MODULE src/BasicBC/BasicBC.cpp)

add_library(llcm_BasicBC_Test0 MODULE src/BasicBC_Test0/BasicBC_Test0.cpp)

add_library(llcm_BasicBC_Vector_Client MODULE src/BasicBC_Vector_Client/BasicBC_Vector_Client.cpp)

add_library(llcm_BasicBC_Vector_Redist_Client MODULE src/BasicBC_Vector_Redist_Client/BasicBC_Vector_Redist_Client.cpp)

add_library(llcm_BasicBC_Vector_Redist_Client2 MODULE src/BasicBC_Vector_Redist_Client2/BasicBC_Vector_Redist_Client2.cpp)

add_library(llcm_BasicBC_Vector_Redist_Server MODULE src/BasicBC_Vector_Redist_Server/BasicBC_Vector_Redist_Server.cpp)

add_library(llcm_BasicBC_Vector_Server MODULE src/BasicBC_Vector_Server/BasicBC_Vector_Server.cpp)

add_library(llcm_ParallelismManager MODULE src/ParallelismManager/ParallelismManager.cpp)

install(TARGETS
	llcm_BasicBC
	llcm_BasicBC_Test0
	llcm_BasicBC_Vector_Client
	llcm_BasicBC_Vector_Redist_Client
	llcm_BasicBC_Vector_Redist_Client2
	llcm_BasicBC_Vector_Redist_Server
	llcm_BasicBC_Vector_Server
	llcm_ParallelismManager
	LIBRARY DESTINATION lib)

install(DIRECTORY include/ DESTINATION include/mxn FILES_MATCHING PATTERN "*.h" PATTERN "*/*" EXCLUDE)
install(DIRECTORY cad/ DESTINATION lib/llcmcpp3/mxn FILES_MATCHING PATTERN "*.llcmcpp3")
