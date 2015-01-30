set(SOURCE_FILES ImagingVectorField.cpp)

add_executable(ImagingVectorField ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(ImagingVectorField ${OpenCV_LIBS})
