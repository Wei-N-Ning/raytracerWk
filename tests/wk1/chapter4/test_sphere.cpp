//
// Created by wein on 26/08/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <ray.hh>
#include <vec3.hh>
#include <ppm.hh>

#include <fstream>

//! P11
//! t * t * dot(B, B) +
//! 2 * t * dot(B, A - c) +
//! dot(A - C, A - C) -
//! R * R = 0
//! the vectors and R are all constant and known,
//! the unknown is t
bool hitSphere( const RTWK1::Ray& ray, const RTWK1::Vec3& center, float radius )
{
    using namespace RTWK1;

    Vec3 centerToOrigin = ray.origin() - center;

    float a = dot( ray.direction(), ray.direction() );

    float b = 2.0f * dot( centerToOrigin, ray.direction() );

    float c = dot( centerToOrigin, centerToOrigin ) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

//! P12
//! "coloring red any pixel that hits a small spehere we place at -1.0
//! on the z-axis (in the view direction)
static constexpr float RADIUS = 0.5f;
RTWK1::Vec3 opaqueSphere( RTWK1::Ray& ray )
{
    RTWK1::Vec3 dir = ray.direction().normalized();
    if ( hitSphere( ray, { 0, 0, -1 }, RADIUS ) )
    {
        return { 1, 0, 0 };
    }
    return RTWK1::generateBackgroundColor( ray );
}

TEST_CASE( "render sphere" )
{
    using namespace RTWK1;

    std::ofstream ofs;
    ofs.open( "/tmp/sphere.ppm" );
    CHECK( ofs.good() );
    createImage( ofs, 200, 100, opaqueSphere );
}
