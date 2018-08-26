
add_executable(test_sphere test_sphere.cpp)
set_target_properties(test_sphere
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_sphere
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter4::test-sphere"
    COMMAND
    test_sphere
)