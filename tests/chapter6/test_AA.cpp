//
// Created by wein on 2/09/18.
//

#include <ppm.hh>
#include <entities.hh>

#include <limits>
#include <fstream>
#include <cassert>

// P19
// For a given pixel we have several samples within that pixel and send
// rays through each of the samples.
// the colors of these rays are then averaged.

RTWK::Vec3 surfaceColor( RTWK::Ray& ray )
{
    static std::vector< RTWK::Sphere > store{
        RTWK::Sphere( RTWK::Vec3{ 0, 0, -1 }, 0.5 ),
        RTWK::Sphere( RTWK::Vec3{ 0, -100.5f, -1 }, 100 ) };

    RTWK::HitableList spheres;
    spheres.add( &( store[ 0 ] ) );
    spheres.add( &( store[ 1 ] ) );

    using namespace RTWK;
    RTWK::HitRecord hitRecord;
    if ( spheres.hit( ray,
                      std::numeric_limits< float >::min(),
                      std::numeric_limits< float >::max(),
                      hitRecord )
         > 0 )
    {
        Vec3 N = hitRecord.normal;
        return 0.5f * Vec3( N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f );
    }
    return backgroundColor( ray );
}

int main()
{
    std::ofstream ofs;
    ofs.open( "/tmp/cameraAA.ppm" );
    assert( ofs.good() );
    RTWK::Camera camera;
    RTWK::createImageCamAA( ofs, 200, 100, 25, camera, surfaceColor );
    return 0;
}