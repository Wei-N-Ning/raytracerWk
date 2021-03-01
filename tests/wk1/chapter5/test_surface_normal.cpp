//
// Created by wein on 26/08/18.
//

#include <ppm.hh>
#include <vec3.hh>
#include <ray.hh>

#include <fstream>
#include <cassert>

//! P13
//! a common trick used for visualizing normals is to map each component
//! to the interval from 0 to 1, and then map x,y,z to r,g,b
//! for the normal we need the hit point, not just whether we hit or
//! not. "Let's assume the closest hit point (smallest t)"
float hitPoint( const RTWK::Vec3& center, float radius, const RTWK::Ray& ray )
{
    using namespace RTWK;

    // C--->P-->(ray.origin)--> (outward)
    Vec3 centerToOrigin = ray.origin() - center;

    float a = dot( ray.direction(), ray.direction() );

    float b = 2.0f * dot( centerToOrigin, ray.direction() );

    float c = dot( centerToOrigin, centerToOrigin ) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if ( discriminant < 0 )
    {
        return -1.0f;
    }
    // compute the smaller root (closer hit point)
    return ( -b - std::sqrt( discriminant ) ) / ( 2.0f * a );
}

constexpr float RADIUS = 0.5f;
RTWK::Vec3 surfaceColor( RTWK::Ray& ray )
{
    using namespace RTWK;

    Vec3 center{ 0, 0, -1 };
    float t = hitPoint( center, RADIUS, ray );
    if ( t > 0 )
    {
        // get the hit point using parameter t
        // recall the implementation of point-at-parameter
        // result is an out-pointing v
        Vec3 N = ( ray.pointAtParameter( t ) - center ).normalized();
        return 0.5f * Vec3( N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f );
    }
    return generateBackgroundColor( ray );
}

int main()
{
    std::ofstream ofs;
    ofs.open( "/tmp/normal.ppm" );
    assert( ofs.good() );
    RTWK::createImage( ofs, 200, 100, surfaceColor );
    return 0;
}