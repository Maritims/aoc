add_executable(day2)
target_sources(day2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day2.c)
target_include_directories(day2 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day2 PRIVATE aoc math4c)
