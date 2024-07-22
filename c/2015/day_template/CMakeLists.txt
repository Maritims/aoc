add_executable(dayX)
target_sources(dayX PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/dayX.c)
target_include_directories(dayX PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(dayX PRIVATE aoc)
