
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

add_executable(test_entity_single_sphere
    test_entity_single_sphere.cpp)
set_target_properties(test_entity_single_sphere
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_entity_single_sphere
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter5::test-hitable-single-sphere"
    COMMAND
    test_entity_single_sphere
)

add_executable(test_entity_multi_spheres
    test_entity_multi_spheres.cpp)
set_target_properties(test_entity_multi_spheres
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(test_entity_multi_spheres
    PUBLIC
    rtwk)
add_test(
    NAME
    "rtwk::chapter5::test-hitable-multi-spheres"
    COMMAND
    test_entity_single_sphere
)