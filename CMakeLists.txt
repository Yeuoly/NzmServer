cmake_minimum_required(VERSION 2.8)

project(nzm-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

aux_source_directory(src/ DIR_SRCS)

add_subdirectory(src/core)
add_subdirectory(src/utils)
add_subdirectory(src/types)

add_executable(build/main ${DIR_SRCS})

target_link_libraries(build/main core)
target_link_libraries(build/main utils)
target_link_libraries(build/main types)