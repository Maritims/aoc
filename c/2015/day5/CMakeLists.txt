add_executable(day5)
target_sources(day5 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day5.c)
target_include_directories(day5 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day5 PRIVATE aoc hashtable)
