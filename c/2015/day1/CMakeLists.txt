add_executable(day1)
target_sources(day1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day1.c)
target_include_directories(day1 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day1 PRIVATE aoc)
