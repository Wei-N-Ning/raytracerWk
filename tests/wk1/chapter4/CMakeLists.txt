
add_executable(test_sphere test_sphere.cpp)
target_link_libraries(test_sphere PUBLIC rtwk1 doctest)
add_test(NAME "rtwk1::chapter4::test-sphere" COMMAND test_sphere)