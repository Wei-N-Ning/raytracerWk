
add_tinytest_executable(test_ray test_ray.cpp)
set_target_properties(test_ray
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_ray
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter3::test-ray"
    COMMAND
    test_ray
)