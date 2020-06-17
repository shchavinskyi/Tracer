# The main executable
add_executable(${EXECUTABLE_NAME} main.cpp) 

# Add source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

list(REMOVE_ITEM SOURCE_FILES main.cpp)

# A library for tests
add_library(${LIB_NAME} ${SOURCE_FILES})

# Remove the lib prefix
set_target_properties(${LIB_NAME} PROPERTIES DBG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Link our sources to the executable
target_link_libraries(${EXECUTABLE_NAME} ${LIB_NAME} )
set_target_properties(${EXECUTABLE_NAME} PROPERTIES DBG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
install(TARGETS ${LIB_NAME} 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
