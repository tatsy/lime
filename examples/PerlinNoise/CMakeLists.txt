set(SOURCE_FILES PerlinNoise.cpp)

add_executable(PerlinNoise ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${OpenCV_DIR}/include)
target_link_libraries(PerlinNoise ${OpenCV_LIBS})
