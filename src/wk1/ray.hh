//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_RAY_HH
#define RAYTRACER_IN_A_WEEKEND_RAY_HH

#include "vec3.hh"

namespace RTWK1
{
//! P8
//! p(t) = A + tB
//! point at parameter, t
//!
//! P9
//! at the core of a ray tracer is to send rays through pixels and
//! compute what color is seen in the direction of those rays.
//! this is of the form
//! - calculate what ray goes from the eye to a pixel,
//! - compute a color for that intersection pointer
//!
//! "what first developing a ray tracer, I always do a simple camera
//! for getting the code up and running. I also make a simple color(ray)
//! function that returns the color of the background (a simple gradient)
//! "
//! "I've often gotten into trouble using square images for debugging
//! because I transpose x and y too often, so I will stick with 200x100
//! image"
class Ray
{
public:
    Ray() = default;
    // p(t) = A + tB
    Ray( Vec3  A, Vec3  B );

    //! A
    const Vec3& origin() const;
    //! B
    const Vec3& direction() const;
    //! A + tB
    Vec3 pointAtParameter( float t ) const;

private:
    Vec3 m_A;
    Vec3 m_B;
};

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_RAY_HH
