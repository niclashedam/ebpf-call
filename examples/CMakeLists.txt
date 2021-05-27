cmake_minimum_required(VERSION 3.16)

project(ebpf-call-example)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-target bpf -g -c")

file(GLOB EXAMPLES "*.c")
add_library(examples OBJECT ${EXAMPLES})
