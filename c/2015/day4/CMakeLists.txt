add_executable(day4)
target_sources(day4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/day4.c)
target_include_directories(day4 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day4 PRIVATE aoc m maritims_md5)
