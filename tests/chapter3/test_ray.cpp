//
// Created by wein on 26/08/18.
//

#include <ray.hh>
#include <vec3.hh>

#include <iostream>
#include <fstream>
#include <cassert>

void RunTinyTests();

//! P10-11
//! The color() function linearly blends white and blue depending on the
//! up/downess of the y coordinate
//! A lerp (linear interpolation) is always of the form
//! blended_value = (1 - t) * start_value + t * end_value
//! with t going from zero to one
RTWK::Vec3 color(const RTWK::Ray& r) {
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

    int xNumPixels = 200;
    int yNumPixels = 100;
    ofs << "P3" << std::endl
        << xNumPixels << " " << yNumPixels << std::endl
        << "255" << std::endl;

    Vec3 lowerLeft(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4, 0, 0);
    Vec3 vertical(0, 2, 0);
    Vec3 origin(0, 0, 0);

    for (int y = yNumPixels - 1; y >= 0; --y) {
        for (int x = 0; x < xNumPixels; ++x) {
            //! P9
            //! "I will reverse the screen from the lower left hand
            //! corner and use two offset vectors along the screen sides
            //! to move the ray endpoint across the screen"
            float u = float(x) / float(xNumPixels);
            float v = float(y) / float(yNumPixels);
            Ray r(origin, lowerLeft + u * horizontal + v * vertical);
            Vec3 c = color(r);
            int ir = int(255.99 * c.r());
            int ig = int(255.99 * c.g());
            int ib = int(255.99 * c.b());

            ofs << ir << " " << ig << " " << ib << std::endl;
        }
    }

}

int main(int argc, char **argv) {
    RunTinyTests();
    return 0;
}
