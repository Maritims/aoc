add_executable(day17)
target_sources(day17 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day17.c)
target_include_directories(day17 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day17 PRIVATE aoc math4c)
