cmake_minimum_required(VERSION 3.29)

project(AdventOfCode2015)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99) # This project uses C99.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)

function(add_aoc_day DAY LIBS)
    if (DAY LESS 1 OR DAY GREATER 25)
        message(FATAL_ERROR "The DAY argument (${DAY}) must be between 1 and 25.")
    endif()

    set(TARGET_NAME "day${DAY}")

    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/days/${TARGET_NAME}.c)
    target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${TARGET_NAME} PRIVATE aoc)

    if (LIBS)
        target_link_libraries(${TARGET_NAME} PRIVATE ${LIBS})
    endif()
endfunction()

function(add_aoc_test TEST_NAME LIBS)
    if (NOT TEST_NAME)
        message(FATAL_ERROR "The TEST_NAME argument is required.")
    endif()

    SET(TARGET_NAME "test_${TEST_NAME}")

    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/tests/${TARGET_NAME}.c)
    target_include_directories(${TARGET_NAME} PRIVATE include)

    if (LIBS)
        target_link_libraries(${TARGET_NAME} ${LIBS})
    endif()

    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

function(add_aoc_library lib_name)
    add_library(${lib_name})
    target_include_directories(${lib_name} PRIVATE include)
endfunction()

add_library(debug INTERFACE)
target_include_directories(debug INTERFACE include/debug.h)

add_aoc_library(aoc)
target_sources(aoc PRIVATE src/aoc.c)
target_link_libraries(aoc file4c string4c)

add_aoc_library(array4c)
target_sources(array4c PRIVATE ${PROJECT_SOURCE_DIR}/src/array4c.c)

add_aoc_library(conway)
target_sources(conway PRIVATE ${PROJECT_SOURCE_DIR}/src/conway.c)

add_aoc_library(file4c)
target_sources(file4c PRIVATE ${PROJECT_SOURCE_DIR}/src/file4c.c)

# add_library(generics)
# target_sources(generics PRIVATE ${PROJECT_SOURCE_DIR}/src/generics/array.c ${PROJECT_SOURCE_DIR}/src/generics/object.c ${PROJECT_SOURCE_DIR}/src/generics/value.c)
# target_include_directories(generics PRIVATE include)

add_aoc_library(grid)
target_sources(grid PRIVATE ${PROJECT_SOURCE_DIR}/src/grid.c)

add_aoc_library(hamiltonian)
target_sources(hamiltonian PRIVATE ${PROJECT_SOURCE_DIR}/src/hamiltonian.c)

add_aoc_library(hashset)
target_sources(hashset PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashset.c)

add_aoc_library(hashtable)
target_sources(hashtable PRIVATE ${PROJECT_SOURCE_DIR}/src/fnv.c ${PROJECT_SOURCE_DIR}/src/hashtable.c)

add_aoc_library(json)
target_sources(json PRIVATE ${PROJECT_SOURCE_DIR}/src/json/lexer.c ${PROJECT_SOURCE_DIR}/src/json/parser.c)
target_link_libraries(json string4c)

add_aoc_library(math4c)
target_sources(math4c PRIVATE ${PROJECT_SOURCE_DIR}/src/math4c.c)
target_link_libraries(math4c array4c gmp m)

add_aoc_library(string4c)
target_sources(string4c PRIVATE ${PROJECT_SOURCE_DIR}/src/string4c.c)
target_link_libraries(string4c math4c)

add_aoc_library(test4c)
target_sources(test4c PRIVATE ${PROJECT_SOURCE_DIR}/src/test4c.c)

add_aoc_library(maritims_md5)
target_sources(maritims_md5 PRIVATE ${PROJECT_SOURCE_DIR}/src/maritims_md5.c)

add_aoc_library(point)
target_sources(point PRIVATE ${PROJECT_SOURCE_DIR}/src/point.c)

# Days
add_aoc_day(1 "")
add_aoc_day(2 "math4c")
add_aoc_day(3 "hashtable")
add_aoc_day(4 "m;maritims_md5")
add_aoc_day(5 "hashtable")
add_aoc_day(6 "point")
add_aoc_day(7 "hashtable;math4c")
add_aoc_day(8 "")
add_aoc_day(9 "hamiltonian")
add_aoc_day(10 "")
add_aoc_day(11 "test4c")
add_aoc_day(12 "json")
add_aoc_day(13 "math4c")
add_aoc_day(14 "")
add_aoc_day(15 "math4c")
add_aoc_day(16 "")
add_aoc_day(17 "math4c")
add_aoc_day(18 "conway;grid")
# add_aoc_day(19 "")

# Enable testing
add_aoc_test(array4c "array4c")
# add_aoc_test(grammar "")
add_aoc_test(grid "grid")
add_aoc_test(hashset "hashset")
add_aoc_test(hashtable "hashtable;string4c")
add_aoc_test(json "json")
add_aoc_test(maritims_md5 "m;maritims_md5")
add_aoc_test(math4c "math4c")
add_aoc_test(point "point")
add_aoc_test(string4c "string4c")

enable_testing()
