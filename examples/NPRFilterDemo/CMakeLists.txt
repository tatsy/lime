set(SOURCE_FILES NPRFilterDemo.cpp)

add_executable(NPRFilterDemo ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(NPRFilterDemo ${OpenCV_LIBS})
