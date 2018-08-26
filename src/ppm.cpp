//
// Created by wein on 26/08/18.
//

#include "ppm.hh"

namespace RTWK {

void createImage(std::ostream &os, int xNumPixels, int yNumPixels) {
    os << "P3" << std::endl
       << xNumPixels << " " << yNumPixels << std::endl
       << "255" << std::endl;
    for (int y = 0; y < yNumPixels; ++y) {
        for (int x = 0; x < xNumPixels; ++x) {
            float r = float(x) / float(xNumPixels);
            float g = float(y) / float(yNumPixels);
            float b = 0.2;
            int ir = int(255.99f * r);
            int ig = int(255.99f * g);
            int ib = int(255.99f * b);
            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

}
