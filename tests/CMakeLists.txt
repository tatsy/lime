enable_testing()

option(SPICA_BUILD_TEST "SPICA_BUILD_TEST" ON)
set(GTEST_ROOT $ENV{GTEST_ROOT} CACHE STRING "GTEST_ROOT")

option(BUILD_LIME_TEST ${BUILD_LIME_TEST} OFF)
if ("${BUILD_LIME_TEST}" STREQUAL "ON")

  if(NOT "${GTEST_ROOT}" STREQUAL "")
    set(GTEST_LIB)
    if(WIN32)
      set(GTEST_LIB "gtest.lib")
      set(GTEST_MAIN_LIB "gtest_main.lib")
    else()
      set(GTEST_LIB "libgtest.a")
      set(GTEST_MAIN_LIB "libgtest_main.a")
    endif()
    set(GTEST_LIBRARY ${GTEST_ROOT}/lib/${GTEST_LIB})
    set(GTEST_MAIN_LIBRARY ${GTEST_ROOT}/lib/${GTEST_MAIN_LIB})
    set(GTEST_INCLUDE_DIRS ${GTEST_ROOT}/include)
  else()
    set(GTEST_LIBRARY $ENV{GTEST_LIBRARY})
    set(GTEST_MAIN_LIBRARY $ENV{GTEST_MAIN_LIBRARY})
    set(GTEST_INCLUDE_DIRS $ENV{GTEST_INCLUDE_DIRS})
  endif()

  add_subdirectory(core)

endif()
