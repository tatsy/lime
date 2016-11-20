set(BUILD_TARGET "pylime")

file(GLOB SOURCE_FILES "*.cpp" "*.h")

include_directories(${OpenCV_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIR}
                    ${PYTHON_INCLUDE_DIR}
                    "${LIME_ROOT}/sources")

add_library(${BUILD_TARGET} SHARED ${SOURCE_FILES})

target_link_libraries(${BUILD_TARGET}
                      ${OpenCV_LIBS}
                      ${Boost_LIBRARIES}
                      ${PYTHON_LIBRARY})
