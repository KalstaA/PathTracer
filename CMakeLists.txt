cmake_minimum_required (VERSION 3.19)
project (PathTracer)
include_directories("src")

# Set the name of the executable to be produced
set(EXECUTABLE_NAME PathTracer)

# Add all .cpp - files under src/ directory
file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
set(CMAKE_BUILD_TYPE Debug)
