add_executable(tracer main.cpp) # The main executable
add_library(tracer-lib hello.cpp) # A library for tests

SET_TARGET_PROPERTIES(tracer-lib PROPERTIES PREFIX "") # Remove the lib prefix

target_link_libraries(tracer tracer-lib) # Link our sources to the executable

install(TARGETS tracer DESTINATION bin)
install(TARGETS tracer-lib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)