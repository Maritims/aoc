add_executable(day18)
target_sources(day18 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day18.c)
target_include_directories(day18 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day18 PRIVATE aoc conway grid)
