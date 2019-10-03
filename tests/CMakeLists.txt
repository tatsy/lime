set(BUILD_TARGET "run_all_tests")

set(LIME_TEST_ROOT ${CMAKE_CURRENT_LIST_DIR})
configure_file(${CMAKE_CURRENT_LIST_DIR}/lime_test.h.in
               ${CMAKE_CURRENT_LIST_DIR}/lime_test.h @ONLY)

set(SOURCE_FILES gtest.cpp lime_test.h)
add_folder(SOURCE_FILES "core")
add_folder(SOURCE_FILES "metrics")

include_directories(${LIME_TEST_ROOT} ${LIME_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})
add_executable(${BUILD_TARGET} ${SOURCE_FILES})
target_link_libraries(${BUILD_TARGET} ${OpenCV_LIBS} ${GTEST_LIBRARIES})

set_target_properties(${BUILD_TARGET} PROPERTIES FOLDER "Tests")
set_target_properties(${BUILD_TARGET} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
source_group("Source Files" FILES ${SOURCE_FILES})

add_test(NAME ${BUILD_TARGET} COMMAND ${BUILD_TARGET})
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose --gtest_shuffle DEPENDS ${BUILD_TARGET})
