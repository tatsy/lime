set(SOURCE_FILES cc_examples.cpp)

add_executable(CCExample ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(CCExample ${OpenCV_LIBS})
