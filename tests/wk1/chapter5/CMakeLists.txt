
add_executable(test_surface_normal test_surface_normal.cpp)
target_link_libraries(test_surface_normal PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter5::test-surface-normal" COMMAND test_surface_normal)

add_executable(test_entity_single_sphere test_entity_single_sphere.cpp)
target_link_libraries(test_entity_single_sphere PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter5::test-hitable-single-sphere" COMMAND test_entity_single_sphere)

add_executable(test_entity_multi_spheres test_entity_multi_spheres.cpp)
target_link_libraries(test_entity_multi_spheres PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter5::test-hitable-multi-spheres" COMMAND test_entity_single_sphere)
