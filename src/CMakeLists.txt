
add_library(rtwk STATIC
    ppm.cpp
    ppm.hh
    vec3.cpp
    vec3.hh
    ray.cpp
    ray.hh
    entities.cpp
    entities.hh
    camera.cpp
    camera.hh
)
target_include_directories(rtwk
    PUBLIC
    .)
set_target_properties(rtwk
    PROPERTIES
    CXX_STANDARD 14)
