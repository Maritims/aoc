add_executable(day3)
target_sources(day3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day3.c)
target_include_directories(day3 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day3 PRIVATE aoc hashtable)
