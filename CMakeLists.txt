cmake_minimum_required(VERSION 2.8.4)
project(lime)

# Required packages
find_package( OpenCV 3.0 REQUIRED )

# Add example directory
add_subdirectory(examples)

# Add test directory
add_subdirectory(tests)
