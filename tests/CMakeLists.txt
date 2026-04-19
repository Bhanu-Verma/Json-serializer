# tests/CMakeLists.txt

# 1. Unit Tests
add_executable(unit_tests json_lib_test.cpp parser_test.cpp)
target_link_libraries(unit_tests PRIVATE json_lib)

# 2. Benchmarks (Use a separate executable)
add_executable(benchmark_parser benchmark_test.cpp)
target_link_libraries(benchmark_parser PRIVATE json_lib)

target_include_directories(unit_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/tester 
    ${CMAKE_SOURCE_DIR}/include
)

# Ensure the benchmark can find your benchmarking header
target_include_directories(benchmark_parser PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optimization for benchmarking
set_target_properties(benchmark_parser PROPERTIES 
    COMPILE_FLAGS "-O3 -march=native"
)