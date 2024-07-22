add_executable(day7)
target_sources(day7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day7.c)
target_include_directories(day7 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day7 PRIVATE aoc array string4c hashtable)
