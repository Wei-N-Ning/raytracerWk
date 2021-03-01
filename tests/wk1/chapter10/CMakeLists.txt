
add_executable(test_poscamera
    test_poscamera.cpp)

set_target_properties(test_poscamera
    PROPERTIES
    CXX_STANDARD 14)

target_link_libraries(test_poscamera
    PUBLIC
    rtwk)

add_test(NAME "test::chapter10::positionable-camera"
    COMMAND test_poscamera)