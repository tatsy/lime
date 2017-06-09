cmake_minimum_required(VERSION 3.0.0)
project(lime)

if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
else()
  set(LINUX FALSE)
endif()

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
  find_package( PythonLibs REQUIRED )
  if (NOT DEFINED PYTHON_INCLUDE_DIR OR NOT DEFINED PYTHON_LIBRARY)
    message(FATAL_ERROR "Python libraries not found. Please set PYTHON_INCLUDE_DIR and PYTHON_LIBRARY manually to CMake.")
  endif()

  message(STATUS "Python ${PYTHONLIBS_VERSION_STRING}")
  message(STATUS "  Include: ${PYTHON_INCLUDE_DIR}")
  message(STATUS "  Library: ${PYTHON_LIBRARY}")

  if (LINUX AND ${PYTHONLIBS_VERSION_STRING} VERSION_GREATER "3.0")
    find_package( Boost REQUIRED python3 numpy3 )
  else()
    find_package( Boost REQUIRED python numpy )
  endif()
endif()

# Build settings
if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")
  if (${LIME_BUILD_TESTS})
    message(STATUS "[ LIME ] Generating coverage data.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
  endif()
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
