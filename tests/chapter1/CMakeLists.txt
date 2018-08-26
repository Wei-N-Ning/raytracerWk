
add_tinytest_executable(test_ppm test_ppm.cpp)
set_target_properties(test_ppm
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_ppm
    PUBLIC
    rtwk)
if (BASH_PROGRAM)
    add_test(
        NAME
        "rtwk::chapter1::test-ppm"
        COMMAND
        ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test_ppm.sh $<TARGET_FILE:test_ppm>)
endif ()