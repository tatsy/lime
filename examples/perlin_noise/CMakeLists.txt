set(SOURCE_FILES perlin_noise.cpp)

add_executable(perlin_noise ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(perlin_noise ${OpenCV_LIBS})
