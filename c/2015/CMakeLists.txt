cmake_minimum_required(VERSION 3.29)

project(AdventOfCode2015)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99) # This project uses C99.

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)

function(add_aoc_library lib_name)
    add_library(${lib_name})
    target_include_directories(${lib_name} PRIVATE include)
endfunction()

add_library(debug INTERFACE)
target_include_directories(debug INTERFACE include/debug.h)

add_aoc_library(aoc)
target_sources(aoc PRIVATE src/aoc.c src/string4c.c)
target_sources(aoc PUBLIC src/file4c.c)

add_aoc_library(array)
target_sources(array PRIVATE ${PROJECT_SOURCE_DIR}/src/array.c)

add_aoc_library(math4c)
target_sources(math4c PRIVATE ${PROJECT_SOURCE_DIR}/src/math4c.c)
target_link_libraries(math4c gmp)

add_aoc_library(string4c)
target_sources(string4c PRIVATE ${PROJECT_SOURCE_DIR}/src/string4c.c)

add_aoc_library(test4c)
target_sources(test4c PRIVATE ${PROJECT_SOURCE_DIR}/src/test4c.c)

add_aoc_library(maritims_md5)
target_sources(maritims_md5 PRIVATE ${PROJECT_SOURCE_DIR}/src/maritims_md5.c)

add_aoc_library(hashset)
target_sources(hashset PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashset.c)

add_aoc_library(hashtable)
target_sources(hashtable PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashtable.c)

add_aoc_library(point)
target_sources(point PRIVATE ${PROJECT_SOURCE_DIR}/src/point.c)

add_aoc_library(grid)
target_sources(grid PRIVATE ${PROJECT_SOURCE_DIR}/src/grid.c)

add_aoc_library(hamiltonian)
target_sources(hamiltonian PRIVATE ${PROJECT_SOURCE_DIR}/src/hamiltonian.c)

add_aoc_library(generics4c)
target_sources(generics4c PRIVATE ${PROJECT_SOURCE_DIR}/src/generics4c.c)

add_aoc_library(json4c)
target_sources(json4c PRIVATE src/json4c.c)
target_link_libraries(json4c generics4c hashtable string4c)

# Enable testing
add_executable(test_hashtable tests/test_hashtable.c src/fnv.c src/hashtable.c)
target_include_directories(test_hashtable PRIVATE include)

add_executable(test_json4c tests/test_json4c.c src/json4c.c)
target_include_directories(test_json4c PRIVATE include)
target_link_libraries(test_json4c generics4c hashtable string4c)

add_executable(test_maritims_md5 tests/test_maritims_md5.c src/maritims_md5.c)
target_include_directories(test_maritims_md5 PRIVATE include)
target_link_libraries(test_maritims_md5 m)

add_executable(test_point tests/test_point.c src/point.c)
target_include_directories(test_point PRIVATE include)

add_executable(test_string4c tests/test_string4c.c src/string4c.c)
target_include_directories(test_string4c PRIVATE include)

enable_testing()

# Add test cases
add_test(NAME HashtableTest COMMAND test_hashtable)
add_test(NAME Json4cTest COMMAND test_json4c)
add_test(NAME MaritimsMD5Test COMMAND test_maritims_md5)
add_test(NAME PointTest COMMAND test_point)
add_test(NAME String4cTest COMMAND test_string4c)

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
add_subdirectory(day13)
add_subdirectory(day14)
add_subdirectory(day15)
