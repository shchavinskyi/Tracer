set(TEST_SOURCES hello_test.cpp)

add_executable(test_runner test_runner.cpp ${TEST_SOURCES})

target_link_libraries(test_runner tracer-lib)

set(DOCTEST_INCLUDE_DIR ../${EXTERNAL_LIBS_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")

message("DOCTEST_INCLUDE_DIR ${DOCTEST_INCLUDE_DIR}")

target_include_directories(test_runner PUBLIC ${DOCTEST_INCLUDE_DIR})
