
add_library(rtwk1exam2 INTERFACE take2_math.hh exam2lib.hh)
target_include_directories(rtwk1exam2 INTERFACE .)

add_executable(test_render_exam2 test_render_take2.cpp)
target_link_libraries(test_render_exam2 PRIVATE rtwk1exam2)
add_test(NAME "rtwk1::exam2::test_render" COMMAND test_render_exam2)

add_executable(lookdev lookdev.cpp)
target_link_libraries(lookdev PRIVATE rtwk1exam2)
