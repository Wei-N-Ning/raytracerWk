//
// Created by wein on 2/09/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <entities.hh>
#include <ppm.hh>

#include <fstream>
#include <limits>
#include <vector>

// RTWK1 P18
// this yields a picture that is really just a visualization of where
// the spheres are along with their surface normal.
// this is often a great way to look at your model for flaws and
// characteristics.

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

TEST_CASE( "render multiple spheres" )
{
    std::ofstream ofs;
    ofs.open( "/tmp/balls.ppm" );
    CHECK( ofs.good() );
    RTWK1::createImage( ofs, 200, 100, surfaceColor );
}
