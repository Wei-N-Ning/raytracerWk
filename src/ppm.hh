//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_PPM_HH
#define RAYTRACER_IN_A_WEEKEND_PPM_HH

#include "ray.hh"
#include "vec3.hh"
#include "camera.hh"

#include <ostream>
#include <functional>

namespace RTWK
{
// Ray function is to abstract the examples in the book so that I can
// reuse the base algorithm (iterating the pixels and writing out color)
// while "plug in" the custom ray-to-color generator function in each
// exercise
// this is used prior to chapter 8, where the concept of material is
// introduced
using RayFunction = std::function< Vec3( Ray& ) >;

// used in chapter 8 to enable material
// also follow the book's example to use the hitable
// "world" to encapsulate the render entities
class IHitable;
class IRender
{
public:
    virtual Vec3 operator()( Ray& ray ) = 0;
};

// The default sky-blue gradient background
Vec3 generateBackgroundColor( Ray& ray );

// used in chapter 1 to test the image driver (ppm)
void createTestImage( std::ostream& os, int xNumPixels, int yNumPixels );

// used until chapter 6 to output image
void createImage( std::ostream& os, int xNumPixels, int yNumPixels, const RayFunction& f );

// used in chapter 6 to test camera and AA
void createImageCamAA( std::ostream& os,
                       int xNumPixels,
                       int yNumPixels,
                       int samplesPerPixel,
                       Camera& cam,
                       const RayFunction& f,
                       bool gammaCorrection = false );

// used in chapter 8 to support material
void createImageMaterial( std::ostream& os,
                          int xNumPixels,
                          int yNumPixels,
                          int samplesPerPixel,
                          Camera& cam,
                          IRender& render,
                          bool gammaCorrection = true );

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_PPM_HH
