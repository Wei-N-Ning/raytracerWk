
add_tinytest_executable(test_vector3 test_vector3.cpp)
set_target_properties(test_vector3
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_vector3
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter2::test-vector3"
    COMMAND
    test_vector3
)