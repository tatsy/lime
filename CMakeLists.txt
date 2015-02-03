cmake_minimum_required(VERSION 2.8.4)
project(lime)

# Required packages
find_package( OpenCV 3.0 REQUIRED )

enable_testing()
# find_package( GTest REQUIRED )

set(GTEST_LIBRARY $ENV{GTEST_LIBRARY})
set(GTEST_INCLUDE_DIRS $ENV{GTEST_INCLUDE_DIRS})

# Add example directory
add_subdirectory(examples)

# Add test directory
enable_testing()
add_subdirectory(tests)
