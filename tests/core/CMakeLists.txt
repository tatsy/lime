set(SOURCE_FILES test_point.cpp)

add_executable(test_point ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${GTest_INCLUDE_DIR})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(test_point ${OpenCV_LIBS})
target_link_libraries(test_point ${GTest_LIBRARY})

add_test(NAME that-test-I-made COMMAND test_point)
