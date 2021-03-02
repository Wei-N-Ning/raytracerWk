
add_executable(test_poscamera test_poscamera.cpp)
target_link_libraries(test_poscamera PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter10::poscamera" COMMAND test_poscamera)