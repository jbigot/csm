project(llcmcpp_components)
cmake_minimum_required(VERSION 2.8)

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(LocalBarrier MODULE src/LocalBarrier/localbarrier.cpp)
target_link_libraries(LocalBarrier ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS LocalBarrier LIBRARY DESTINATION lib)

install(DIRECTORY include/ DESTINATION include/threads FILES_MATCHING PATTERN "*.h" PATTERN "*/*" EXCLUDE)
install(DIRECTORY cad/ DESTINATION lib/llcmcpp3/threads FILES_MATCHING PATTERN "*.llcmcpp3")
