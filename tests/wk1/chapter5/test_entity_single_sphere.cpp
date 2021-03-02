//
// Created by wein on 27/08/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <entities.hh>
#include <ppm.hh>

#include <fstream>
#include <limits>

RTWK1::Vec3 surfaceColor( RTWK1::Ray& ray )
{
    using namespace RTWK1;

    RTWK1::Sphere sphere( { 0, 0, -1 }, 0.5 );
    RTWK1::HitRecord hitRecord;
    if ( sphere.hit( ray,
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

TEST_CASE( "render surface color" )
{
    std::ofstream ofs;
    ofs.open( "/tmp/entity.ppm" );
    CHECK( ofs.good() );
    RTWK1::createImage( ofs, 200, 100, surfaceColor );
}
