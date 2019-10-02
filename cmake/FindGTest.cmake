include(FindPackageHandleStandardArgs)

set(GTest_DIR "GTest_DIR" CACHE PATH "")

if (WIN32)
    find_path(GTEST_INCLUDE_DIR
              NAMES gtest/gtest.h
              PATHS
              ${GTest_DIR}
              ${GTest_DIR}/include)

    find_library(GTEST_LIBRARY
                 NAMES gtest
                 PATHS
                 ${GTest_DIR}
                 ${GTest_DIR}/lib
                 ${GTest_DIR}/lib/Release)

    find_library(GTEST_MAIN_LIBRARY
                 NAMES gtest_main
                 PATHS
                 ${GTest_DIR}
                 ${GTest_DIR}/lib
                 ${GTest_DIR}/lib/Release)
else()
    find_path(GTEST_INCLUDE_DIR
              NAMES gtest/gtest.h
              PATHS
              /usr/include
              /usr/local/include
              ${GTest_DIR}
              ${GTest_DIR}/include)

    find_library(GTEST_LIBRARY
                 NAMES gtest
                 PATHS
                 /usr/lib
                 /usr/local/lib
                 /usr/lib/x86_64-linux-gnu
                 ${GTest_DIR}
                 ${GTest_DIR}/lib)

    find_library(GTEST_MAIN_LIBRARY
                 NAMES gtest_main
                 PATHS
                 /usr/lib
                 /usr/local/lib
                 /usr/lib/x86_64-linux-gnu
                 ${GTest_DIR}
                 ${GTest_DIR}/lib
                 ${GTest_DIR}/lib/Release)
endif()

find_package_handle_standard_args(
    GTest DEFAULT_MSG
    GTEST_INCLUDE_DIR
    GTEST_LIBRARY
    GTEST_MAIN_LIBRARY
)

if (GTest_FOUND)
    message(STATUS "GTest include: ${GTEST_INCLUDE_DIR}")
    message(STATUS "GTest library: ${GTEST_LIBRARY}")
    message(STATUS "GTest main library: ${GTEST_MAIN_LIBRARY}")
    set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
    set(GTEST_LIBRARIES ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
endif()

mark_as_advanced(GTEST_INCLUDE_DIR GTEST_LIBRARY GTEST_MAIN_LIBRARY)