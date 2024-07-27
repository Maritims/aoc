add_executable(day13)
target_sources(day13 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day13.c)
target_include_directories(day13 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day13 PRIVATE aoc math4c)
