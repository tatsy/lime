cmake_minimum_required(VERSION 3.0.0)
project(lime)

# Required packages
find_package( OpenCV 3.0 REQUIRED )

# Build settings
if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC")
endif()

if (WIN32)
  set(LIB_PREFIX "")
  set(LIB_SUFFIX ".lib")
else()
  set(LIB_PREFIX "lib")
  set(LIB_SUFFIX ".so")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add example directory
set(LIME_ROOT ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(sources)
add_subdirectory(examples)

# Add test directory
add_subdirectory(tests)
