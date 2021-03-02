

add_executable(test_material test_material.cpp)
target_link_libraries(test_material PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter8::test-material" COMMAND test_material)