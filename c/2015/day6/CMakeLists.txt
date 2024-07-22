add_executable(day6)
target_sources(day6 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day6.c)
target_include_directories(day6 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day6 PRIVATE aoc grid point)
