cmake_minimum_required(VERSION 3.10)  # CMake version check
project(mazeSolver)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(ORIGINAL OriginalMazeSolver.cpp)
set(PARALLEL ParallelMazeSolver.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(sequential ${ORIGINAL})
add_executable(parallel ${PARALLEL})
