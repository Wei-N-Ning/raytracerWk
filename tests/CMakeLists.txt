
add_tinytest_executable(test_ppm image/test_ppm.cpp)
set_target_properties(test_ppm
    PROPERTIES
    CXX_STANDARD 14
)
add_test(
    NAME
    "rtwk::image::test-ppm"
    COMMAND
    test_ppm
)
