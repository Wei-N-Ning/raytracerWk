

add_executable(test_metal test_metal.cpp)
set_target_properties(test_metal
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_metal
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter8::metal-material"
    COMMAND
    test_metal
)