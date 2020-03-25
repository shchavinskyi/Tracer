
# Add source files
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp)

set(TEST_EXECUTABLE_NAME test_runner${CMAKE_DEBUG_POSTFIX})

add_executable(${TEST_EXECUTABLE_NAME} test_runner.cpp ${TEST_SOURCES})

target_link_libraries(${TEST_EXECUTABLE_NAME} ${LIB_NAME})

set(DOCTEST_INCLUDE_DIR ${EXTERNAL_LIBS_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")

target_include_directories(${TEST_EXECUTABLE_NAME} PUBLIC ${DOCTEST_INCLUDE_DIR})
