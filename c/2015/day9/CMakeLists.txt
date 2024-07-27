add_executable(day9)
target_sources(day9 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day9.c)
target_include_directories(day9 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day9 PRIVATE aoc math4c string4c hashset hamiltonian)
