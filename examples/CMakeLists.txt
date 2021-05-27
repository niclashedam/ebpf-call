cmake_minimum_required(VERSION 3.16)

project(ebpf-call-example)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-target bpf -g -c")

file(GLOB EXAMPLES "*.c")
add_library(examples OBJECT ${EXAMPLES})


foreach(EXAMPLE ${EXAMPLES})
  string(REPLACE ".c" "" NAME ${EXAMPLE})
  string(REPLACE ${CMAKE_SOURCE_DIR}/examples/ "" NAME ${NAME})
  string(REPLACE "/" "_" TARGET ${NAME})

  add_custom_target(example_${TARGET}
    ALL
    DEPENDS examples
    COMMAND cp "${CMAKE_SOURCE_DIR}/examples/CMakeFiles/objects.dir/${NAME}.c.o" ${NAME}.o
    BYPRODUCTS ${NAME}.o
    COMMENT "Moving ${NAME} object file .. (${NAME}.o)"
  )
endforeach()
