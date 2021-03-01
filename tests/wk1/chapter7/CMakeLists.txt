
add_executable(test_diffuse test_diffuse.cpp)
set_target_properties(test_diffuse
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_diffuse
    PUBLIC
    rtwk
    )
add_test(
    NAME
    "rtwk::chapter7::diffuse-materials"
    COMMAND
    test_diffuse
)


