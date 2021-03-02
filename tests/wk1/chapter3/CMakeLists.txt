
add_executable(test_ray test_ray.cpp)
target_link_libraries(test_ray PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter3::test-ray" COMMAND test_ray)