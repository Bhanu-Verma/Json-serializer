# Add an executable for the example
add_executable(usage_example simple_usage_example.cpp)
add_executable(parse_example example_parse_with_benchmark.cpp)


# Link it to your library
target_link_libraries(usage_example PRIVATE json_lib)
target_link_libraries(parse_example PRIVATE json_lib)

# Copy data file to the build directory so the executable can find it
configure_file(${CMAKE_SOURCE_DIR}/large.json ${CMAKE_BINARY_DIR}/examples/large.json COPYONLY)

# Ensure it can find your headers
target_include_directories(usage_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(parse_example PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)