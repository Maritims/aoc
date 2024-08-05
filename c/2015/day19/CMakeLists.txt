add_executable(day19)
target_sources(day19 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day19.c)
target_include_directories(day19 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day19 PRIVATE aoc generics hashtable string4c)
