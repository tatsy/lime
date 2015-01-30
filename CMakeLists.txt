cmake_minimum_required(VERSION 2.8.4)
project(lime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

# required libraries (OpenCV 3.0 or higher)
FIND_PACKAGE( OpenCV REQUIRED )

add_subdirectory(examples)
