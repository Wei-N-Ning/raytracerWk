
add_executable(test_render test_render.cpp)
target_link_libraries(test_render PRIVATE doctest)
add_test(NAME "rtwk1::exam1::test_render" COMMAND test_render)
