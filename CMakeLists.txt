cmake_minimum_required(VERSION 3.16)

project (ebpf-call)

add_subdirectory(examples)

find_library(ubpf REQUIRED)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/**.c")
add_executable(ebpf-call src/loader.c ${SOURCES})

target_link_libraries(ebpf-call PUBLIC ubpf)
