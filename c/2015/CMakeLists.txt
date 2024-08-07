cmake_minimum_required(VERSION 3.29)

project(AdventOfCode2015)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99) # This project uses C99.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
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

add_aoc_library(array4c)
target_sources(array4c PRIVATE ${PROJECT_SOURCE_DIR}/src/array4c.c)

add_aoc_library(conway)
target_sources(conway PRIVATE ${PROJECT_SOURCE_DIR}/src/conway.c)

add_library(generics)
target_sources(generics PRIVATE ${PROJECT_SOURCE_DIR}/src/generics/array.c ${PROJECT_SOURCE_DIR}/src/generics/object.c ${PROJECT_SOURCE_DIR}/src/generics/value.c)
target_include_directories(generics PRIVATE include)

add_aoc_library(grid)
target_sources(grid PRIVATE ${PROJECT_SOURCE_DIR}/src/grid.c)

add_aoc_library(hamiltonian)
target_sources(hamiltonian PRIVATE ${PROJECT_SOURCE_DIR}/src/hamiltonian.c)

add_aoc_library(hashset)
target_sources(hashset PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashset.c)

add_aoc_library(hashtable)
target_sources(hashtable PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashtable.c)

add_aoc_library(json)
target_sources(json PRIVATE ${PROJECT_SOURCE_DIR}/src/json/lexer.c ${PROJECT_SOURCE_DIR}/src/json/parser.c ${PROJECT_SOURCE_DIR}/src/json/token.c)
target_link_libraries(json generics string4c)

add_aoc_library(math4c)
target_sources(math4c PRIVATE ${PROJECT_SOURCE_DIR}/src/math4c.c)
target_link_libraries(math4c array4c gmp m)

add_aoc_library(string4c)
target_sources(string4c PRIVATE ${PROJECT_SOURCE_DIR}/src/string4c.c)

add_aoc_library(test4c)
target_sources(test4c PRIVATE ${PROJECT_SOURCE_DIR}/src/test4c.c)

add_aoc_library(maritims_md5)
target_sources(maritims_md5 PRIVATE ${PROJECT_SOURCE_DIR}/src/maritims_md5.c)

add_aoc_library(point)
target_sources(point PRIVATE ${PROJECT_SOURCE_DIR}/src/point.c)

# Enable testing
add_executable(test_array4c tests/test_array4c.c src/array4c.c)
target_include_directories(test_array4c PRIVATE include)

add_executable(test_grammar tests/test_grammar.c src/grammar/grammar.c src/grammar/rule.c)
target_include_directories(test_grammar PRIVATE include)
target_link_libraries(test_grammar hashset hashtable string4c)

add_executable(test_grid tests/test_grid.c src/grid.c)
target_include_directories(test_grid PRIVATE include)

add_executable(test_hashset tests/test_hashset.c src/fnv.c src/hashset.c)
target_include_directories(test_hashset PRIVATE include)

add_executable(test_hashtable tests/test_hashtable.c src/fnv.c src/hashtable.c)
target_include_directories(test_hashtable PRIVATE include)
target_link_libraries(test_hashtable string4c)

add_executable(test_json tests/test_json.c)
target_include_directories(test_json PRIVATE include)
target_link_libraries(test_json json generics)

add_executable(test_maritims_md5 tests/test_maritims_md5.c src/maritims_md5.c)
target_include_directories(test_maritims_md5 PRIVATE include)
target_link_libraries(test_maritims_md5 m)

add_executable(test_math4c tests/test_math4c.c src/array4c.c src/math4c.c)
target_include_directories(test_math4c PRIVATE include)
target_link_libraries(test_math4c gmp m)

add_executable(test_point tests/test_point.c src/point.c)
target_include_directories(test_point PRIVATE include)

add_executable(test_string4c tests/test_string4c.c src/string4c.c)
target_include_directories(test_string4c PRIVATE include)

enable_testing()

# Add test cases
# add_test(NAME Array4CTest COMMAND test_array4c)
add_test(NAME GrammarTest COMMAND test_grammar)
add_test(NAME GridTest COMMAND test_grid)
add_test(NAME HashSetTest COMMAND test_hashset)
add_test(NAME HashTableTest COMMAND test_hashtable)
add_test(NAME JsonTest COMMAND test_json)
add_test(NAME MaritimsMD5Test COMMAND test_maritims_md5)
add_test(NAME Math4CTest COMMAND test_math4c)
add_test(NAME PointTest COMMAND test_point)
add_test(NAME String4CTest COMMAND test_string4c)

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
add_subdirectory(day16)
add_subdirectory(day17)
add_subdirectory(day18)
add_subdirectory(day19)
