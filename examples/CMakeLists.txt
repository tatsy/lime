# Example generate function
function(generate_example example_name example_sources)
  add_executable(${example_name} "${example_sources}")

  # include_directories(${CMAKE_CURRENT_LIST_DIR})
  include_directories(${OpenCV_DIR}/include)
  target_link_libraries(${example_name} ${OpenCV_LIBS})

  if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    set_property(TARGET ${example_name} APPEND PROPERTY LINK_FLAGS "/DEBUG /PROFILE")
    set_property(TARGET ${example_name} PROPERTY FOLDER "Examples")
  endif()
endfunction()

option(LIME_BUILD_EXAMPLE "LIME_BUILD_EXAMPLE" ON)
if ("${LIME_BUILD_EXAMPLE}" STREQUAL "ON")

endif()

add_subdirectory(cc_example)
add_subdirectory(flow_field_design)
add_subdirectory(npr_filter_demo)
add_subdirectory(pencil_drawing)
add_subdirectory(perlin_noise)
add_subdirectory(vector_field_imaging)
