set(SOURCE_FILES flow_field_design.cpp)

add_executable(flow_field_design ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(flow_field_design ${OpenCV_LIBS})
