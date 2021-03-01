
add_executable(test_ppm test_ppm.cpp)
target_link_libraries(test_ppm PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter1::test_image" COMMAND test_ppm)