//
// Created by wein on 2/09/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <ppm.hh>
#include <entities.hh>

#include <limits>
#include <fstream>

// P19
// For a given pixel we have several samples within that pixel and send
// rays through each of the samples.
// the colors of these rays are then averaged.

RTWK1::Vec3 surfaceColor( RTWK1::Ray& ray )
{
    static std::vector< RTWK1::Sphere > store{
        RTWK1::Sphere( RTWK1::Vec3{ 0, 0, -1 }, 0.5 ),
        RTWK1::Sphere( RTWK1::Vec3{ 0, -100.5f, -1 }, 100 ) };

    RTWK1::HitableList spheres;
    spheres.add( &( store[ 0 ] ) );
    spheres.add( &( store[ 1 ] ) );

    using namespace RTWK1;
    RTWK1::HitRecord hitRecord;
    if ( spheres.hit( ray,
                      std::numeric_limits< float >::min(),
                      std::numeric_limits< float >::max(),
                      hitRecord )
         > 0 )
    {
        Vec3 N = hitRecord.normal;
        return 0.5f * Vec3( N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f );
    }
    return generateBackgroundColor( ray );
}

TEST_CASE( "test camera and anti-aliasing" )
{
    std::ofstream ofs;
    ofs.open( "/tmp/cameraAA.ppm" );
    CHECK( ofs.good() );
    RTWK1::Camera camera;
    RTWK1::createImageCamAA( ofs, 200, 100, 25, camera, surfaceColor );
}