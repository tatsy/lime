configure_file("${CMAKE_CURRENT_LIST_DIR}/core/version.h.in"
               "${CMAKE_CURRENT_LIST_DIR}/core/version.h" @ONLY)

include_directories(${LIME_INCLUDE_DIR})

if (${LIME_BUILD_STATIC_LIB})
  message(STATUS "[ LIME ] Build static library.")

  set(SOURCE_FILES lime.hpp lime.cpp)
  add_folder(SOURCE_FILES core INSTALL)
  add_folder(SOURCE_FILES npr INSTALL)
  add_folder(SOURCE_FILES misc INSTALL)
  add_folder(SOURCE_FILES metrics INSTALL)
  
  include_directories(${OpenCV_INCLUDE_DIRS})
  add_library(${LIME_LIBRARY} STATIC ${SOURCE_FILES})
  target_compile_definitions(${LIME_LIBRARY} PRIVATE "-DLIME_BUILD_LIBRARY")
  target_link_libraries(${LIME_LIBRARY} ${OpenCV_LIBS})

  source_group("Source Files" FILES ${SOURCE_FILES})

  install(TARGETS ${LIME_LIBRARY}
          EXPORT LimeTargets
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION bin
          ARCHIVE DESTINATION lib)

  install(FILES lime.hpp
          EXPORT LimeTargets
          DESTINATION include)
endif()

if (${LIME_BUILD_PYTHON_MODULE})
  add_subdirectory(python)
endif()
