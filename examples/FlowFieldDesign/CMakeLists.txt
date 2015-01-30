set(SOURCE_FILES FlowFieldDesign.cpp)

add_executable(FlowFieldDesign ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(FlowFieldDesign ${OpenCV_LIBS})
