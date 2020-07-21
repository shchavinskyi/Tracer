cmake_minimum_required(VERSION 3.2)

project(tracer VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_TESTS "Build test executable" ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as no build type was specified")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type (Debug/Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pthread -fno-rtti -fno-exceptions -Wno-stringop-truncation")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # disable rtti
    if(CMAKE_CXX_FLAGS MATCHES "/GR ")
        string(REGEX REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /GR-")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /GR- /EHsc /WX -Wall -Wextra -Werror -Wno-c++98-compat-pedantic -Wno-format-nonliteral -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-c++98-compat-pedantic -pthread -fno-rtti -fno-exceptions -Wno-missing-braces -Wmissing-field-initializers")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX _debug)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(EXECUTABLE_NAME tracer)
set(LIB_NAME tracer-lib)

set(EXTERNAL_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

include_directories(include)
include_directories(${EXTERNAL_LIBS_DIR}) 
include_directories(${EXTERNAL_LIBS_DIR}/stb) 
include_directories(${EXTERNAL_LIBS_DIR}/glm) 

add_subdirectory(src)

if (BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif (BUILD_TESTS)
