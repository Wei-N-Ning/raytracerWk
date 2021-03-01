//
// Created by wein on 26/08/18.
//

#include "ppm.hh"
#include "vec3.hh"

#include <random>

namespace RTWK1
{
Vec3 generateBackgroundColor( Ray& r )
{
    RTWK1::Vec3 dir = r.direction().normalized();
    // center to horizon
    float t = 0.5f * ( dir.y() + 1.0f );
    // a two-tune blend function using t as the parameter, ranging (0.0f, 1.0f)
    return ( 1.0f - t ) * RTWK1::Vec3( 1.0, 1.0, 1.0 ) + t * RTWK1::Vec3( 0.5, 0.7, 1.0 );
}

void createTestImage( std::ostream& os, int xNumPixels, int yNumPixels )
{
    os << "P3" << std::endl
       << xNumPixels << " " << yNumPixels << std::endl
       << "255" << std::endl;
    for ( int y = 0; y < yNumPixels; ++y )
    {
        for ( int x = 0; x < xNumPixels; ++x )
        {
            // from top-left corner, 0, 0 to bottom-right corner 255, 255
            // blend <0.0, 0.0, 0.0> and <255., 255., 255.,>
            Vec3 color{
                float( x ) / float( xNumPixels ), float( y ) / float( yNumPixels ), 0.2 };
            int ir = int( 255.99f * color.r() );
            int ig = int( 255.99f * color.g() );
            int ib = int( 255.99f * color.b() );
            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

void createImage( std::ostream& os, int xNumPixels, int yNumPixels, const RayFunction& f )
{
    os << "P3" << std::endl
       << xNumPixels << " " << yNumPixels << std::endl
       << "255" << std::endl;

    Vec3 lowerLeft( -2.0f, -1.0f, -1.0f );
    Vec3 horizontal( 4, 0, 0 );
    Vec3 vertical( 0, 2, 0 );
    Vec3 origin( 0, 0, 0 );

    for ( int y = yNumPixels - 1; y >= 0; --y )
    {
        for ( int x = 0; x < xNumPixels; ++x )
        {
            //! P9
            //! "I will reverse the screen from the lower left hand
            //! corner and use two offset vectors along the screen sides
            //! to move the ray endpoint across the screen"
            float u = float( x ) / float( xNumPixels );
            float v = float( y ) / float( yNumPixels );
            Ray r( origin, lowerLeft + u * horizontal + v * vertical );
            Vec3 c = f( r );
            int ir = int( 255.99 * c.r() );
            int ig = int( 255.99 * c.g() );
            int ib = int( 255.99 * c.b() );

            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

// P23
// almost all image viewers assume that the image is gamma-corrected,
// meaning the 0 to 1 values have some transform before being stored
// as a byte
// there are many good reasons for that....
// to a first approximation, we can use "gamma 2" which means raising
// the color to the power 1/gamma, or 1/2, which is just square root
void gammaCorrected( Vec3& col )
{
    col = Vec3( sqrt( col.r() ), sqrt( col.g() ), sqrt( col.b() ) );
}

void createImageCamAA( std::ostream& os,
                       int xNumPixels,
                       int yNumPixels,
                       int samplesPerPixel,
                       Camera& cam,
                       const RayFunction& rayFunction,
                       bool gammaCorrection )
{
    os << "P3" << std::endl
       << xNumPixels << " " << yNumPixels << std::endl
       << "255" << std::endl;
    std::mt19937 generator{ std::random_device()() };
    std::uniform_real_distribution< double > dist( 0, 1 );

    for ( int y = yNumPixels - 1; y >= 0; --y )
    {
        for ( int x = 0; x < xNumPixels; ++x )
        {
            Vec3 outColor{ 0, 0, 0 };

            for ( int sample = 0; sample < samplesPerPixel; ++sample )
            {
                // sub pixel (0, 1) sampling
                float u = float( x + dist( generator ) ) / float( xNumPixels );
                float v = float( y + dist( generator ) ) / float( yNumPixels );
                Ray r = cam.getRay( u, v );
                outColor += rayFunction( r );
            }
            outColor /= float( samplesPerPixel );
            if ( gammaCorrection )
            {
                gammaCorrected( outColor );
            }
            int ir = int( 255.99 * outColor.r() );
            int ig = int( 255.99 * outColor.g() );
            int ib = int( 255.99 * outColor.b() );

            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

void createImageMaterial( std::ostream& os,
                          int xNumPixels,
                          int yNumPixels,
                          int samplesPerPixel,
                          Camera& cam,
                          IRender& renderer,
                          bool gammaCorrection )
{
    os << "P3" << std::endl
       << xNumPixels << " " << yNumPixels << std::endl
       << "255" << std::endl;
    std::mt19937 generator{ std::random_device()() };
    std::uniform_real_distribution< double > dist( 0, 1 );

    for ( int y = yNumPixels - 1; y >= 0; --y )
    {
        for ( int x = 0; x < xNumPixels; ++x )
        {
            Vec3 outColor{ 0, 0, 0 };

            for ( int sample = 0; sample < samplesPerPixel; ++sample )
            {
                float u = float( x + dist( generator ) ) / float( xNumPixels );
                float v = float( y + dist( generator ) ) / float( yNumPixels );
                Ray r = cam.getRay( u, v );
                outColor += renderer.render( r );
            }
            outColor /= float( samplesPerPixel );
            if ( gammaCorrection )
            {
                gammaCorrected( outColor );
            }
            int ir = int( 255.99 * outColor.r() );
            int ig = int( 255.99 * outColor.g() );
            int ib = int( 255.99 * outColor.b() );

            os << ir << " " << ig << " " << ib << std::endl;
        }
    }
}

}  // namespace RTWK
