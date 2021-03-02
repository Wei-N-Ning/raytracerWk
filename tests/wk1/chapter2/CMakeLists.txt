
add_executable(test_vector3 test_vector3.cpp)
target_link_libraries(test_vector3 PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter2::test-vector3" COMMAND test_vector3)