# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(EPollServer VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable for your program
add_executable(EPollServer EPollServer.cpp)

# Link the pthread library
target_link_libraries(EPollServer pthread)

# Specify the include directory where ctpl_stl.h is located
target_include_directories(EPollServer PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optional: Compiler flags for optimization/debugging
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Compiling in Debug mode")
    set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic")
else()
    message("Compiling in Release mode")
    set(CMAKE_CXX_FLAGS "-O2")
endif()

# Optional: Output directories for compiled binary
set_target_properties(EPollServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
