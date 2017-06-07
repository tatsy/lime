if (${LIME_BUILD_STATIC_LIB})
  message(STATUS "[ LIME ] Build static library.")

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

  configure_file("${CMAKE_CURRENT_LIST_DIR}/core/version.h.in"
                 "${CMAKE_CURRENT_LIST_DIR}/core/version.h" @ONLY)

  add_folder(core "*.cpp" "*.h" "*.hpp")
  add_folder(misc "*.cpp" "*.h" "*.hpp")
  add_folder(npr "*.cpp" "*.h" "*.hpp")
  add_library(lime ${SOURCE_FILES})

  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries(lime ${OpenCV_LIBS})
endif()

if (${LIME_BUILD_PYTHON_MODULE})
  add_subdirectory(python)
endif()
