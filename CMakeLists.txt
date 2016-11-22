cmake_minimum_required(VERSION 3.0.0)
project(lime)

# ------------------------------------------------------------------------------
# Build options
# ------------------------------------------------------------------------------
option(LIME_BUILD_TESTS "Build unit tests." OFF)
option(LIME_BUILD_EXAMPLES "Build example programes." ON)
option(LIME_BUILD_STATIC_LIB "Build static library." OFF)
option(LIME_BUILD_PYTHON_MODULE "Build python modules." OFF)

# ------------------------------------------------------------------------------
# Required packages
# ------------------------------------------------------------------------------
find_package( OpenCV 3.0 REQUIRED )
if (${LIME_BUILD_PYTHON_MODULE})
  find_package( PythonLibs 3 REQUIRED )
  find_package( Boost REQUIRED python numpy )
endif()

# Build settings
if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")
endif()

if (LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC")
endif()

# ------------------------------------------------------------------------------
# Set library prefices and suffices
# ------------------------------------------------------------------------------
if (WIN32)
  set(LIB_PREFIX "")
  set(LIB_SUFFIX ".lib")
elseif(APPLE)
  if (SPICA_USE_STATIC_LIBS)
    set(LIB_PREFIX "lib")
    set(LIB_SUFFIX ".a")
  else()
    set(LIB_PREFIX "lib")
    set(LIB_SUFFIX ".dylib")
  endif()
else()
  if (SPICA_USE_STATIC_LIBS)
    set(LIB_PREFIX "lib")
    set(LIB_SUFFIX ".a")
  else()
    set(LIB_PREFIX "lib")
    set(LIB_SUFFIX ".so")
  endif()
endif()

# ------------------------------------------------------------------------------
# Set output path
# ------------------------------------------------------------------------------
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add example directory
set(LIME_ROOT ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(sources)
add_subdirectory(examples)

# Add test directory
add_subdirectory(tests)
