

add_executable(test_material test_material.cpp)
set_target_properties(test_material
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_material
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter8::test-material"
    COMMAND
    test_material
)