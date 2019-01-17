cmake_minimum_required(VERSION 3.5)

project(t3l C)

file (GLOB_RECURSE SOURCES "*.c")
file (GLOB_RECURSE HEADERS "../include/*.h")

set(GCC_COVERAGE_COMPILE_FLAGS "-g")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../include
)
add_executable(t3l ${SOURCES} ${HEADERS})
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
