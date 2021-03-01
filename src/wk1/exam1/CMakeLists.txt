
add_executable(test_render test_render.cpp)
target_link_libraries(test_render PRIVATE doctest)
add_test(NAME "exam::test_render" COMMAND test_render)

add_executable(test_render_take2 test_render_take2.cpp)
add_test(NAME "exam::test_render_take2" COMMAND test_render_take2)

add_executable(test_render_take2_math test_render_take2_math.cpp)
add_test(NAME "exam::test_render_take2_math" COMMAND test_render_take2_math)