add_executable(day15)
target_sources(day15 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day15.c)
target_include_directories(day15 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day15 PRIVATE aoc)
