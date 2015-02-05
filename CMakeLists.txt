cmake_minimum_required(VERSION 2.8.4)
project(lime)

# Required packages
find_package( OpenCV 3.0 REQUIRED )

# Build settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

# Add example directory
add_subdirectory(examples)

# Add test directory
add_subdirectory(tests)
