add_executable(day16)
target_sources(day16 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day16.c)
target_include_directories(day16 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day16 PRIVATE aoc hashtable)
