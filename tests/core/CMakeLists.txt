# Define function to add test
function(add_gtest_with_opencv test_name test_source)
  set(SOURCE_FILES "${test_source}")

  add_executable(${test_name} ${SOURCE_FILES})
  target_link_libraries(${test_name} ${GTEST_LIBRARY})
  target_link_libraries(${test_name} ${GTEST_MAIN_LIBRARY})
  target_link_libraries(${test_name} ${OpenCV_LIBS})

  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction(add_gtest_with_opencv)

set(TEST_SOURCES ${TEST_SOURCES}
    test_core.cpp
    test_point.cpp
    test_random.cpp
    test_random_queue.cpp
    test_array2d.cpp)

add_gtest_with_opencv(test_core "${TEST_SOURCES}")

# Add tests to "make check"
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_core)

# Include directories
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
