cmake_minimum_required(VERSION 2.8.4)
project(lime)

#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

# Required packages
find_package( OpenCV 3.0 REQUIRED )
find_package( GTest REQUIRED )

# Add sub-directories
add_subdirectory(examples)
add_subdirectory(tests)
