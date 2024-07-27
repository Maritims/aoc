add_executable(day11)
target_sources(day11 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day11.c)
target_include_directories(day11 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day11 PRIVATE aoc string4c test4c)
