
add_executable(test_render test_render.cpp)
target_link_libraries(test_render PRIVATE doctest)
add_test(NAME "exam::test_render" COMMAND test_render)

add_executable(test_render_take2 test_render_take2.cpp)
target_link_libraries(test_render_take2 PRIVATE doctest)
add_test(NAME "exam::test_render_take2" COMMAND test_render_take2)
