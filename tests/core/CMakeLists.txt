# Test for Point
set(POINT_SOURCE_FILES test_point.cpp)

add_executable(test_point ${POINT_SOURCE_FILES})
target_link_libraries(test_point ${GTEST_LIBRARY})
target_link_libraries(test_point ${GTEST_MAIN_LIBRARY})
target_link_libraries(test_point ${OpenCV_LIBS})

add_test(NAME test_point COMMAND test_point)

# Test for Random
set(RANDOM_SOURCE_FILES test_random.cpp)

add_executable(test_random ${RANDOM_SOURCE_FILES})
target_link_libraries(test_random ${GTEST_LIBRARY})
target_link_libraries(test_random ${GTEST_MAIN_LIBRARY})
target_link_libraries(test_random ${OpenCV_LIBS})

add_test(NAME test_random COMMAND test_random)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_point test_random)

# Include directories
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
