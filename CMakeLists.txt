cmake_minimum_required(VERSION 3.19)
project(PathTracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include_directories("src")

# Create executable
set(EXECUTABLE_NAME PathTracer)
file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Get SFML library from GitHub
include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics)

# Get yaml-cpp library from GitHub
include(FetchContent)
FetchContent_Declare(yaml-cpp GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git GIT_TAG yaml-cpp-0.7.0)
FetchContent_MakeAvailable(yaml-cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE yaml-cpp)

# OpenMP
find_package(OpenMP COMPONENTS CXX REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenMP::OpenMP_CXX)

# Eigen
include_directories(${CMAKE_SOURCE_DIR}/libs/include)
