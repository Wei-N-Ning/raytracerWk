//
// Created by wein on 26/08/18.
//

#include <ray.hh>
#include <vec3.hh>
#include <ppm.hh>

#include <iostream>
#include <fstream>
#include <cassert>

void RunTinyTests();

//! To be plugged in to createImage() function
//!
//! P10-11
//! The color() function linearly blends white and blue depending on the
//! up/downess of the y coordinate
//! A lerp (linear interpolation) is always of the form
//! blended_value = (1 - t) * start_value + t * end_value
//! with t going from zero to one
RTWK::Vec3 simpleGradient(RTWK::Ray& r) {
    RTWK::Vec3 dir = r.direction().normalized();
    // from un-normalized to normalized [-1.0, 1.0]
    // then shifted to [0, 2.0] and scaled down to
    // [0, 1.0]
    // (0.5, 0.7, 1.0) is sort of sky blue
    float t = 0.5f * (dir.y() + 1.0f);
    return (1.0f - t) * RTWK::Vec3(1.0, 1.0, 1.0) +
           t * RTWK::Vec3(0.5, 0.7, 1.0);
}

void test_ctor() {
    using namespace RTWK;
    Vec3 a{1, 2, 3}, b{4, 5, 6};
    Ray r(a, b);
}

void test_color() {
    using namespace RTWK;

    std::ofstream ofs;
    ofs.open("/tmp/color.ppm");
    assert(ofs.good());
    createImage(ofs, 200, 100, simpleGradient);
}

int main(int argc, char **argv) {
    RunTinyTests();
    return 0;
}
