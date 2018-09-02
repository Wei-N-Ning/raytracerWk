//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_PPM_HH
#define RAYTRACER_IN_A_WEEKEND_PPM_HH

#include "ray.hh"
#include "vec3.hh"
#include "camera.hh"

#include <ostream>

namespace RTWK {

// Ray function is to abstract the examples in the book so that I can
// reuse the base algorithm (iterating the pixels and writing out color)
// while "plug in" the custom ray-to-color generator function in each
// exercise
using RayFunction = Vec3 (Ray&);

// The default sky-blue gradient background
Vec3 backgroundColor(Ray& ray);

// used in chapter 1 to test the image driver (ppm)
void createTestImage(
    std::ostream &os,
    int xNumPixels, int yNumPixels);

// used until chapter 6 to output image
void createImage(
    std::ostream &os,
    int xNumPixels, int yNumPixels, RayFunction f);

// used in chapter 6 to test camera and AA
void createImageCamAA(
    std::ostream &os,
    int xNumPixels, int yNumPixels, int samplesPerPixel,
    Camera& cam, RayFunction f,
    bool gammaCorrection = false);

}

#endif //RAYTRACER_IN_A_WEEKEND_PPM_HH
