
# Add source files
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp)

set(TEST_EXECUTABLE_NAME test_runner)

add_executable(${TEST_EXECUTABLE_NAME} test_runner.cpp ${TEST_SOURCES})

set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES DBG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${TEST_EXECUTABLE_NAME} ${LIB_NAME})

set(DOCTEST_INCLUDE_DIR ${EXTERNAL_LIBS_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")

target_include_directories(${TEST_EXECUTABLE_NAME} PUBLIC ${DOCTEST_INCLUDE_DIR})
