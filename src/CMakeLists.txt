# Collect all source files
file(GLOB SOURCES "*.cpp")

# Create the library
add_library(json_lib ${SOURCES})

# Define the interface: Users include the 'include' directory
target_include_directories(json_lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

# Optional: Add a library alias so it's easier to find in larger projects
add_library(json::json_lib ALIAS json_lib)