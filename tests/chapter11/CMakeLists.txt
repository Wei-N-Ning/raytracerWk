

add_executable(test_depth_of_field
    test_depth_of_field.cpp)

set_target_properties(test_depth_of_field
    PROPERTIES
    CXX_STANDARD 14)

target_link_libraries(test_depth_of_field
    PUBLIC
    rtwk)

add_test(NAME "test::chapter10::depth-of-field"
    COMMAND test_depth_of_field)