cmake_minimum_required(VERSION 3.0.0)
project(lime)

if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
else()
  set(LINUX FALSE)
endif()

# ------------------------------------------------------------------------------
# Set version string
# ------------------------------------------------------------------------------
set(LIME_VERSION_STRING "0.3.0")
message(STATUS "LIME v${LIME_VERSION_STRING}")
file(WRITE "VERSION" "${LIME_VERSION_STRING}")

# ------------------------------------------------------------------------------
# Build options
# ------------------------------------------------------------------------------
option(LIME_BUILD_TESTS "Build unit tests." OFF)
option(LIME_BUILD_EXAMPLES "Build example programes." ON)
option(LIME_BUILD_STATIC_LIB "Build static library." OFF)
option(LIME_BUILD_PYTHON_MODULE "Build python modules." OFF)
option(WITH_OPENMP "Build with OpenMP" ON)

# ------------------------------------------------------------------------------
# Required packages
# ------------------------------------------------------------------------------
find_package(OpenCV 3.0 REQUIRED)
if (WITH_OPENMP)
  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
endif()

# ------------------------------------------------------------------------------
# Build settings
# ------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
if (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

if (${LIME_BUILD_TESTS})
  find_package(GTest REQUIRED)
  if (NOT WIN32)
    message(STATUS "[ LIME ] Generating coverage data.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
  endif()
endif()

# ------------------------------------------------------------------------------
# Set output path
# ------------------------------------------------------------------------------
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_DEBUG_POSTFIX "-debug")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add example directory
set(LIME_ROOT ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(sources)
add_subdirectory(examples)

# Add test directory
add_subdirectory(tests)
