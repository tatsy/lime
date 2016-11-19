set(LIME_BUILD_STATIC_LIB OFF CACHE BOOL OFF)
if (${LIME_BUILD_STATIC_LIB} STREQUAL "ON")
  set(SOURCE_FILES "")
  function(add_folder FOLDER_NAME)
    set(filters "")
    foreach(f ${ARGN})
      set(filters ${filters} "${FOLDER_NAME}/${f}")
    endforeach()

    file(GLOB LOCAL_FILES ${filters})
    set(SOURCE_FILES ${SOURCE_FILES} ${LOCAL_FILES} PARENT_SCOPE)

    source_group(${FOLDER_NAME} FILES ${LOCAL_FILES})
  endfunction()

  add_folder(core "*.cpp" "*.h" "*.hpp")
  add_folder(misc "*.cpp" "*.h" "*.hpp")
  add_folder(npr "*.cpp" "*.h" "*.hpp")
  add_library(lime ${SOURCE_FILES})

  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries(lime ${OpenCV_LIBS})
endif()
