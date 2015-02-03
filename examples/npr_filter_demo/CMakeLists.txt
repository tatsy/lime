set(SOURCE_FILES npr_filter_demo.cpp)

add_executable(npr_filter_demo ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(npr_filter_demo ${OpenCV_LIBS})
