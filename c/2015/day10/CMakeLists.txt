add_executable(day10)
target_sources(day10 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day10.c)
target_include_directories(day10 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day10 PRIVATE aoc)
