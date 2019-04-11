cmake_minimum_required(VERSION 3.5)

project(t3l C)

file (GLOB_RECURSE SOURCES "*.c")
file (GLOB_RECURSE HEADERS "../include/*.h")
file (GLOB_RECURSE LIBS    "../lib/*.*")

set(GCC_COVERAGE_COMPILE_FLAGS "-g -Wall")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../include
)
add_executable(t3l ${SOURCES} ${HEADERS})
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
target_link_libraries(t3l ${LIBS})
