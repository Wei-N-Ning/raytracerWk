//
// Created by wein on 2/09/18.
//

#include <entities.hh>
#include <ppm.hh>

#include <fstream>
#include <limits>
#include <vector>
#include <cassert>

// RTWK P18
// this yields a picture that is really just a visualization of where
// the spheres are along with their surface normal.
// this is often a great way to look at your model for flaws and
// characteristics.

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
    ofs.open( "/tmp/balls.ppm" );
    assert( ofs.good() );
    RTWK::createImage( ofs, 200, 100, surfaceColor );
    return 0;
}
