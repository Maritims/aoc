add_executable(day14)
target_sources(day14 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day14.c)
target_include_directories(day14 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day14 PRIVATE aoc)
