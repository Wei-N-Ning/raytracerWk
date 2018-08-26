
add_executable(test_surface_normal test_surface_normal.cpp)
set_target_properties(test_surface_normal
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_surface_normal
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter5::test-surface-normal"
    COMMAND
    test_surface_normal
)