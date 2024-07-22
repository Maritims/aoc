cmake_minimum_required(VERSION 3.29)

# This project is C only.
project(AdventOfCode2015)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)
find_package(OpenSSL REQUIRED)

function(add_aoc_library lib_name)
    add_library(${lib_name})
    target_include_directories(${lib_name} PRIVATE include)
endfunction()

add_library(debug INTERFACE)
target_include_directories(debug INTERFACE include/debug.h)

add_aoc_library(aoc)
target_sources(aoc PRIVATE src/aoc.c src/string4c.c src/get_file.c)

add_aoc_library(array)
target_sources(array PRIVATE ${PROJECT_SOURCE_DIR}/src/array.c)

add_aoc_library(bitutils)
target_sources(bitutils PRIVATE ${PROJECT_SOURCE_DIR}/src/bitutils.c)

add_aoc_library(math4c)
target_sources(math4c PRIVATE ${PROJECT_SOURCE_DIR}/src/math4c.c)

add_aoc_library(string4c)
target_sources(string4c PRIVATE ${PROJECT_SOURCE_DIR}/src/string4c.c)

add_aoc_library(test4c)
target_sources(test4c PRIVATE ${PROJECT_SOURCE_DIR}/src/test4c.c)

add_aoc_library(maritims_md5)
target_sources(maritims_md5 PRIVATE ${PROJECT_SOURCE_DIR}/src/maritims_md5.c)
target_link_libraries(maritims_md5 bitutils math4c)

add_aoc_library(hashset)
target_sources(hashset PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashset.c)

add_aoc_library(hashtable)
target_sources(hashtable PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashtable.c)

add_aoc_library(point)
target_sources(point PRIVATE ${PROJECT_SOURCE_DIR}/src/point.c)

add_aoc_library(grid)
target_sources(grid PRIVATE ${PROJECT_SOURCE_DIR}/src/grid.c)

add_aoc_library(hamiltonian_path)
target_sources(hamiltonian_path PRIVATE ${PROJECT_SOURCE_DIR}/src/hamiltonian_path.c)

add_aoc_library(json4c)
target_sources(json4c PRIVATE src/json4c.c src/generics4c.c)
target_link_libraries(json4c hashtable string4c)

add_subdirectory(tests)
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
