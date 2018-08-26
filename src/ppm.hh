//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_PPM_HH
#define RAYTRACER_IN_A_WEEKEND_PPM_HH

#include "ray.hh"
#include "vec3.hh"

#include <ostream>

namespace RTWK {

// Ray function is to abstract the examples in the book so that I can
// reuse the base algorithm (iterating the pixels and writing out color)
// while "plug in" the custom ray-to-color generator function in each
// exercise
using RayFunction = Vec3 (Ray&);

void createTestImage(
    std::ostream &os,
    int xNumPixels, int yNumPixels);

void createImage(
    std::ostream &os,
    int xNumPixels, int yNumPixels, RayFunction f);

}

#endif //RAYTRACER_IN_A_WEEKEND_PPM_HH
