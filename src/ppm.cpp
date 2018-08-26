//
// Created by wein on 26/08/18.
//

#include "ppm.hh"
#include "vec3.hh"

namespace RTWK {

void createImage(std::ostream &os, int xNumPixels, int yNumPixels) {
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

}
