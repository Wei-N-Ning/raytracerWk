
add_executable(test_dielectrics
    test_dielectrics.cpp)

set_target_properties(test_dielectrics
    PROPERTIES
    CXX_STANDARD 14)

target_link_libraries(test_dielectrics
    PUBLIC
    rtwk)

add_test(NAME
    "rtwk::chapter9::test-dielectrics"
    COMMAND test_dielectrics)