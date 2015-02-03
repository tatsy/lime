set(SOURCE_FILES test_point.cpp)

add_executable(test_point ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(test_point ${OpenCV_LIBS})
target_link_libraries(test_point ${GTEST_LIBRARY})

add_test(NAME test_point COMMAND test_point)
