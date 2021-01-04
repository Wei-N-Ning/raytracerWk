//
// Created by wein on 26/08/18.
//

#include <ray.hh>
#include <vec3.hh>
#include <ppm.hh>

#include <fstream>
#include <cassert>

//! P11
//! t * t * dot(B, B) +
//! 2 * t * dot(B, A - c) +
//! dot(A - C, A - C) -
//! R * R = 0
//! the vectors and R are all constant and known,
//! the unknown is t
bool hitSphere( const RTWK::Ray& ray, const RTWK::Vec3& center, float radius )
{
    using namespace RTWK;

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
RTWK::Vec3 opaqueSphere( RTWK::Ray& ray )
{
    RTWK::Vec3 dir = ray.direction().normalized();
    if ( hitSphere( ray, { 0, 0, -1 }, RADIUS ) )
    {
        return { 1, 0, 0 };
    }
    return RTWK::backgroundColor( ray );
}

void test_renderSphere()
{
    using namespace RTWK;

    std::ofstream ofs;
    ofs.open( "/tmp/sphere.ppm" );
    assert( ofs.good() );
    createImage( ofs, 200, 100, opaqueSphere );
}

int main()
{
    test_renderSphere();
    return 0;
}