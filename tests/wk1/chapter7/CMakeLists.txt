
add_executable(test_diffuse test_diffuse.cpp)
target_link_libraries(test_diffuse PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter7::diffuse-materials" COMMAND test_diffuse)
