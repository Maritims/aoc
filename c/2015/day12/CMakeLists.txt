add_executable(day12)
target_sources(day12 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day12.c)
target_include_directories(day12 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day12 PRIVATE aoc generics json test4c)
