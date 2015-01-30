set(SOURCE_FILES main.cpp PencilDrawing.cpp)

add_executable(PencilDrawing ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(PencilDrawing ${OpenCV_LIBS})
