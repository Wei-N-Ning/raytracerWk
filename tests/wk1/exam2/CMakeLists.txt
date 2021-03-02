
add_executable(test_render_take2_math test_render_take2_math.cpp)
target_link_libraries(test_render_take2_math PRIVATE rtwk1exam2)
add_test(NAME "rtwk1::exam2::test_math_utils" COMMAND test_render_take2_math)