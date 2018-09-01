
add_executable(test_AA test_AA.cpp)
set_target_properties(test_AA
    PROPERTIES
    CXX_STANDARD 14)
add_test(
    NAME
    "rtwk::chapter6::antialiasing"
    COMMAND
    test_AA
)