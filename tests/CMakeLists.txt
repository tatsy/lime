enable_testing()

set(GTEST_ROOT $ENV{GTEST_ROOT} CACHE STRING "GTEST_ROOT")

set(LIME_BUILD_TEST OFF CACHE BOOL OFF)
if ("${LIME_BUILD_TEST}" STREQUAL "ON")

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

  include_directories("${LIME_ROOT}/sources")
  add_subdirectory(core)

endif()
