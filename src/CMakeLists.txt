
add_library(rtwk OBJECT
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
    material.cpp
    material.hh
    simpleRender.cpp
    simpleRender.hh
    )
target_include_directories(rtwk PUBLIC .)
