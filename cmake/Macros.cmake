# ----------
# Add source codes in the folder
# ----------
function(add_folder SOURCES FOLDER_NAME)
    set(options INSTALL)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(add_folder "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(GLOB LOCAL_FILES
         "${FOLDER_NAME}/*.cpp" "${FOLDER_NAME}/*.hpp"
         "${FOLDER_NAME}/*.c" "${FOLDER_NAME}/*.h")
    set(${SOURCES} ${${SOURCES}} ${LOCAL_FILES} PARENT_SCOPE)

    source_group(${FOLDER_NAME} FILES ${LOCAL_FILES})

    if (add_folder_INSTALL)
        message(STATUS "Install \"${FOLDER_NAME}\"")
        install(FILES ${LOCAL_FILES}
                EXPORT LimeTargets
                DESTINATION "include/${FOLDER_NAME}")
    endif()
endfunction()

# ----------
# Add example module
# ----------
function(add_example EXPNAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(add_example "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(TARGET_EXAMPLE_DIR "${CMAKE_CURRENT_LIST_DIR}/${EXPNAME}")

    file(GLOB SOURCE_FILES "${EXPNAME}/*.cpp" "${EXPNAME}/*.h")

    include_directories(${LIME_INCLUDE_DIR})

    message(STATUS "Example: ${EXPNAME}")
    add_executable(${EXPNAME} ${SOURCE_FILES})

    link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    target_link_libraries(${EXPNAME} ${OpenCV_LIBS})
    if (LIME_BUILD_STATIC_LIB)
        target_link_libraries(${EXPNAME} ${LIME_LIBRARY})
        add_dependencies(${EXPNAME} ${LIME_LIBRARY})
    endif()

    set_target_properties(${EXPNAME} PROPERTIES FOLDER "Examples")
    set_target_properties(${EXPNAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    source_group("Source Files" FILES ${SOURCE_FILES})

    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
        set_property(TARGET ${EXPNAME} APPEND PROPERTY LINK_FLAGS "/DEBUG /PROFILE")
    endif()
endfunction()