option(BUILD_STATIC_LIB ${BUILD_STATIC_LIB} "OFF")
if ("${BUILD_STATIC_LIB}" STREQUAL "ON")

  set(SOURCES ${SOURCES} lime.cpp)
  set(HEADERS ${HEADERS})

  add_subdirectory(core)
  add_subdirectory(hvs)
  add_subdirectory(inpaint)
  add_subdirectory(npr)

  add_library(lime SHARED ${SOURCES} ${HEADERS})

  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries(lime ${OpenCV_LIBS})
endif()
