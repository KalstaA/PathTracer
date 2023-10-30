cmake_minimum_required(VERSION 3.19)
project(CMakeSFMLProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
include_directories("src")

# Set the name of the executable
set(EXECUTABLE_NAME PathTracer)

file(GLOB SOURCES src/*.cpp)
#set(CMAKE_BUILD_TYPE Debug)

# Add all .cpp - files under src/ directory
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Link yaml-cpp library
target_link_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/yaml-cpp-master/buildlib)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE yaml-cpp)

# Get SFML library from github
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics)


install(TARGETS PathTracer)
