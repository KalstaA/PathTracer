cmake_minimum_required(VERSION 3.19)
project(PathTracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
include_directories("src")

# Set the name of the executable
set(EXECUTABLE_NAME PathTracer)

file(GLOB SOURCES src/*.cpp)
#set(CMAKE_BUILD_TYPE Debug)

# Add all .cpp - files under src/ directory
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Link yaml-cpp library
include_directories(${CMAKE_SOURCE_DIR}/libs/yaml/include)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/yaml/lib/libyaml-cpp.a)

# Get SFML library from github
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics)

# Detect platform
if(WIN32)
   message("You are using Windows - no parallelization for you")
elseif(APPLE)
   message("You are using MacOS")

    # OpenMP - works only for Mac for now
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -fopenmp")
    include_directories(${CMAKE_SOURCE_DIR}/libs/libomp/include)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/libomp/lib/libomp.a)

elseif(UNIX)
    message("You are using Linux")
endif()

# Eigen
include_directories(${CMAKE_SOURCE_DIR}/libs/eigen/include)

install(TARGETS PathTracer)
