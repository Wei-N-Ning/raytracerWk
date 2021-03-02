add_executable(test_depth_of_field test_depth_of_field.cpp)
target_link_libraries(test_depth_of_field PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter10::dof" COMMAND test_depth_of_field)