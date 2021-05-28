cmake_minimum_required(VERSION 3.16)

project (ebpf-call)

set(CMAKE_BUILD_TYPE Debug)

if(NOT BACKEND)
    set(BACKEND mmap)
endif()

add_subdirectory(examples)

find_library(ubpf REQUIRED)
find_library(pthread REQUIRED)

message(STATUS "Backend: ${BACKEND}")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/modules/*.c")
add_executable(ebpf-call ${SOURCES} src/loader.c  ./src/backends/${BACKEND}.c)

target_link_libraries(ebpf-call PUBLIC ubpf)
target_link_libraries(ebpf-call PUBLIC pthread)
