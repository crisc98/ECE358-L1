# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ECE358-L1")

# Specify the C++ standard
# See https://cmake.org/cmake/help/latest/guide/tutorial/index.html#specify-the-c-standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output folder where your program will be created
# See https://tuannguyen68.gitbooks.io/learning-cmake-a-beginner-s-guide/content/chap1/chap1.html.
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Include sub-projects.
add_subdirectory ("src")
