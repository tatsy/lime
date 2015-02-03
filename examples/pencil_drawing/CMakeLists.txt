set(SOURCE_FILES main.cpp pencil_drawing.cpp)

add_executable(pencil_drawing ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(pencil_drawing ${OpenCV_LIBS})
