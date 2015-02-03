set(SOURCE_FILES vector_field_imaging.cpp)

add_executable(vector_field_imaging ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(vector_field_imaging ${OpenCV_LIBS})
