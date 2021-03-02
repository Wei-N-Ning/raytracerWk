
add_executable(test_dielectrics test_dielectrics.cpp)
target_link_libraries(test_dielectrics PRIVATE rtwk1 doctest)
add_test(NAME "rtwk1::chapter9::test-dielectrics" COMMAND test_dielectrics)