add_executable(hashtable_tests)
target_sources(hashtable_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/hashtable_tests.c)
target_include_directories(hashtable_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(hashtable_tests PRIVATE hashtable)

add_executable(md5_tests)
target_sources(md5_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/maritims_md5_tests.c)
target_include_directories(md5_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(md5_tests PRIVATE maritims_md5 bitutils m)

add_executable(bitutils_tests)
target_sources(bitutils_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/bitutils_tests.c)
target_include_directories(bitutils_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(bitutils_tests PRIVATE bitutils)

add_executable(point_tests)
target_sources(point_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/point_tests.c)
target_include_directories(bitutils_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(point_tests point)

add_executable(json4c_tests)
target_sources(json4c_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/json4c_tests.c)
target_include_directories(json4c_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(json4c_tests json4c)
