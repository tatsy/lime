set(BUILD_TARGET "pylime")

if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/pybind11)
    message(FATAL_ERROR "Submodule \"pybind11\" is not cloned yet.")
endif()

add_subdirectory(pybind11)

file(GLOB SOURCE_FILES "*.cpp" "*.h")

include_directories(${LIME_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS})

pybind11_add_module(${BUILD_TARGET} ${SOURCE_FILES})

target_link_libraries(${BUILD_TARGET} PRIVATE
                      ${OpenCV_LIBS})
