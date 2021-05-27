cmake_minimum_required(VERSION 3.16)

project(ebpf-call-example)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-target bpf -g -c")

file(GLOB EXAMPLES "*.c")
add_library(objects OBJECT ${EXAMPLES})


foreach(EXAMPLE ${EXAMPLES})
  string(REPLACE ".c" "" NAME ${EXAMPLE})
  string(REPLACE ${CMAKE_SOURCE_DIR}/ "" NAME ${NAME})

  add_custom_target(move_${NAME}
    ALL
    DEPENDS objects
    COMMAND cp "${CMAKE_SOURCE_DIR}/CMakeFiles/objects.dir/${NAME}.c.o" ${NAME}.o
    BYPRODUCTS ${NAME}.o
    COMMENT "Moving ${NAME} object file .. (${NAME}.o)"
  )
endforeach()
