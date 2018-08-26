//
// Created by wein on 26/08/18.
//

#include "ppm.hh"
#include "vec3.hh"

namespace RTWK {

void createTestImage(std::ostream &os, int xNumPixels, int yNumPixels) {
    os << "P3" << std::endl
       << xNumPixels << " " << yNumPixels << std::endl
       << "255" << std::endl;
    for (int y = 0; y < yNumPixels; ++y) {
        for (int x = 0; x < xNumPixels; ++x) {
            Vec3 color{float(x) / float(xNumPixels),
                       float(y) / float(yNumPixels),
                       0.2};
            int ir = int(255.99f * color.r());
            int ig = int(255.99f * color.g());
            int ib = int(255.99f * color.b());
            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

void createImage(
    std::ostream &os,
    int xNumPixels, int yNumPixels, RayFunction f) {

    os << "P3" << std::endl
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
            Vec3 c = f(r);
            int ir = int(255.99 * c.r());
            int ig = int(255.99 * c.g());
            int ib = int(255.99 * c.b());

            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

}
