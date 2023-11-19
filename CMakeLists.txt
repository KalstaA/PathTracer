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

# Unit tests
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(TESTS unittests)
enable_testing()
add_executable(${TESTS} tests/AllTests.cc)
target_link_libraries(${TESTS} PRIVATE gtest_main)
target_link_libraries(${TESTS} PRIVATE sfml-graphics)
include(GoogleTest)
gtest_discover_tests(${TESTS})