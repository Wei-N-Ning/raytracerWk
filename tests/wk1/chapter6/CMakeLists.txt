
add_executable(test_AA test_AA.cpp)
target_link_libraries(test_AA PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter6::antialiasing" COMMAND test_AA)