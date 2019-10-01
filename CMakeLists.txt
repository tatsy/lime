cmake_minimum_required(VERSION 3.0.0)
project(lime)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(Macros)

if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
else()
  set(LINUX FALSE)
endif()

# ------------------------------------------------------------------------------
# Set version string
# ------------------------------------------------------------------------------
set(LIME_MAJOR_VERSION 0)
set(LIME_MINOR_VERSION 3)
set(LIME_PATCH_VERSION 0)
set(LIME_VERSION "${LIME_MAJOR_VERSION}.${LIME_MINOR_VERSION}.${LIME_PATCH_VERSION}")
message(STATUS "LIME v${LIME_VERSION}")
file(WRITE "VERSION" "${LIME_VERSION}")

# ------------------------------------------------------------------------------
# Common paths
# ------------------------------------------------------------------------------
set(LIME_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/sources)
set(LIME_LIBRARY lime)

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
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
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

# ------------------------------------------------------------------------------
# Traverse source directory
# ------------------------------------------------------------------------------
add_subdirectory(sources)

# ------------------------------------------------------------------------------
# Add example directory
# ------------------------------------------------------------------------------
if (${LIME_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

# ------------------------------------------------------------------------------
# Add test directory
# ------------------------------------------------------------------------------
if (${LIME_BUILD_TESTS})
  find_package(GTest REQUIRED)
  if (NOT WIN32)
    message(STATUS "[ LIME ] Generating coverage data.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
  endif()

  enable_testing()
  add_subdirectory(tests)
endif()

# ------------------------------------------------------------------------------
# Installation setup
# ------------------------------------------------------------------------------
if (LIME_BUILD_STATIC_LIB)
  export(TARGETS lime FILE "${CMAKE_BINARY_DIR}/LimeTargets.cmake")
  export(PACKAGE lime)

  set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
  set(INSTALL_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")
  configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/LimeConfig.cmake.in
                ${CMAKE_BINARY_DIR}/LimeConfig.cmake @ONLY)

  configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/LimeConfigVersion.cmake.in
                ${CMAKE_BINARY_DIR}/LimeConfigVersion.cmake @ONLY)

  install(FILES
          ${CMAKE_BINARY_DIR}/LimeConfig.cmake
          ${CMAKE_BINARY_DIR}/LimeConfigVersion.cmake
          DESTINATION ${CMAKE_INSTALL_PREFIX}
          COMPONENT dev)

  install(EXPORT LimeTargets
          DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT dev)
endif()