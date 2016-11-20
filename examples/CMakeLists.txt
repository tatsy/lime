# Example generate function
function(generate_example EXAMPLE_NAME)
  file(GLOB SOURCE_FILES "${EXAMPLE_NAME}/*.cpp" "${EXAMPLE_NAME}*.h")

  set(TARGET_NAME "demo_${EXAMPLE_NAME}")
  add_executable(${TARGET_NAME} ${SOURCE_FILES})

  include_directories("${LIME_ROOT}/sources")
  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS})

  set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Examples")
  if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY LINK_FLAGS "/DEBUG /PROFILE")
  endif()
endfunction()

if (${LIME_BUILD_EXAMPLES})
  message(STATUS "[ LIME ] Build examples.")
  generate_example(color_constancy)
  generate_example(flow_field_design)
  generate_example(npr_filters)
  generate_example(pencil_drawing)
  generate_example(perlin_noise)
  generate_example(vector_field_imaging)
endif()
