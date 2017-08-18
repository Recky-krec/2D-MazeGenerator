cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

project(MazePrototype)

# Includes
file(GLOB_RECURSE myIncludes "include/*") #"dependencies/*")
include_directories(include)
#include_directories(dependencies)

# Sources
file(GLOB_RECURSE mySources "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${mySources} include/Cell.h src/Cell.cpp)

# Libraries for SFML and OpenGL
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
