cmake_minimum_required(VERSION 3.15)
project(FastJsonLibrary VERSION 1.0.0 LANGUAGES CXX)

# Force C++20 for transparent unordered_map lookups
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization flags for Release builds
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -Ofast is excellent for math-heavy parsing and serialization
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
endif()

# Include the src folder
add_subdirectory(src)
add_subdirectory(examples)

# Option to build tests
option(BUILD_TESTING "Build the test suite" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()