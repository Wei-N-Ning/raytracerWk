//
// Created by weining on 28/1/21.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <vector>
#include <ostream>
#include <iostream>
#include <memory>
#include <optional>

using Pixel = int;

using Ray = double;

using Vec3 = double;

struct Camera
{
    [[nodiscard]] Ray getRay( double u, double v ) const
    {
        return {};
    }
};

struct HitRecord;

struct ScatterRecord
{
    Ray ray{};
    Vec3 attenuation{};
};

struct Material
{
    std::optional< ScatterRecord > scattered( const Ray& in, const HitRecord& hitRecord )
    {
        return std::nullopt;
    }
};

struct HitRecord
{
    Material material{};
    Vec3 normal{};
    Vec3 hitPoint{};
    double t{};
};

struct RangeLimit
{
    double min{};
    double max{};

    RangeLimit& update( double newMax )
    {
        max = newMax;
        return *this;
    }
};

struct IHitable
{
    virtual std::optional< HitRecord > hitTest( const Ray& ray,
                                                const RangeLimit& limit ) const = 0;
};

struct HitableList : public IHitable
{
    std::vector< IHitable* > pHitables{};

    void add( IHitable* ptr )
    {
        pHitables.emplace_back( ptr );
    }

    [[nodiscard]] std::optional< HitRecord > hitTest(
        const Ray& ray,
        const RangeLimit& limit ) const override
    {
        std::optional< HitRecord > ret{};
        RangeLimit li{ limit };
        for ( const auto& pHitable : pHitables )
        {
            if ( auto optRecord = pHitable->hitTest( ray, li ); optRecord )
            {
                const auto& hitRecord = *optRecord;
                li.update( hitRecord.t );
                ret.emplace( hitRecord );
            }
        }
        return ret;
    }
};

struct Sphere : public IHitable
{
    [[nodiscard]] std::optional< HitRecord > hitTest(
        const Ray& ray,
        const RangeLimit& limit ) const override
    {
        return std::nullopt;
    }
};

struct Renderer
{
    HitableList hitableList{};

    void add( IHitable* ptr )
    {
        hitableList.add( ptr );
    }

    [[nodiscard]] Pixel generateBackgroundColor( const Ray& ray ) const
    {
        return 42;
    }

    [[nodiscard]] Pixel render( const Ray& ray ) const
    {
        RangeLimit limit{ 0, 10000000 };
        if ( auto optRecord = hitableList.hitTest( ray, limit ); optRecord )
        {
            auto& hitRecord = *optRecord;
            if ( auto optScaRecord = hitRecord.material.scattered( ray, hitRecord );
                 optScaRecord )
            {
                const auto& scatterRecord = *optScaRecord;
                return scatterRecord.attenuation * render( scatterRecord.ray );
            }
            else
            {
                return {};  // absorbed, black hole
            }
        }
        return generateBackgroundColor( ray );
    }
};

using Status = bool;

struct ImageDriver
{
    std::vector< Pixel > pixels{};
    int xNumPixels{ 4 };
    int yNumPixels{ 4 };
    size_t subSamples{ 1 };

    Status drive( const Camera& cam, const Renderer& ren )
    {
        pixels.resize( xNumPixels * yNumPixels );
        for ( auto y = yNumPixels - 1; y >= 0; --y )
        {
            for ( auto x = 0; x < xNumPixels; ++x )
            {
                Pixel pixel{};
                for ( auto ss = 0; ss < subSamples; ++ss )
                {
                    double u = double( x /*with random offset*/ ) / double( xNumPixels );
                    double v = double( y /*with random offset*/ ) / double( yNumPixels );
                    pixel += ren.render( cam.getRay( u, v ) );
                }
                pixels[ x + y * xNumPixels ] =
                    postProcess( pixel / double( subSamples ) );
            }
        }
        return true;
    }

    [[nodiscard]] Pixel postProcess( Pixel pixel ) const
    {
        return pixel;
    }

    std::ostream& output( std::ostream& os ) const
    {
        os << "P3\n" << xNumPixels << " " << yNumPixels << "\n255\n";
        std::for_each( std::cbegin( pixels ),
                       std::cend( pixels ),
                       [ w = xNumPixels, &os, idx = 1 ]( const auto& pixel ) mutable {
                           os << pixel << ( idx++ % w == 0 ? '\n' : ' ' );
                       } );
        return os;
    }
};

TEST_CASE( "ensure it compiles" )
{
    std::ostringstream oss;
    ImageDriver id;
    Sphere sphere{};
    Renderer renderer{};
    renderer.add( &sphere );
    if ( auto status = id.drive( Camera{}, renderer ); status )
    {
        id.output( oss );
        std::cout << oss.str() << '\n';
    }
    else
    {
        std::cout << "failed to render\n";
    }
}
