add_executable(day8)
target_sources(day8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day8.c)
target_include_directories(day8 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day8 PRIVATE aoc string4c)
