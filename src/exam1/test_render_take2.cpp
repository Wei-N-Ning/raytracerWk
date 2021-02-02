//
// Created by weining on 28/1/21.
//

#include "take2_math.hh"

#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <optional>
#include <random>
#include <cassert>
#include <algorithm>

struct Camera
{
    double width{};
    double height{};
    Vec3 position{};
    Vec3 lookAt{};
    Vec3 up{};

    Camera() = default;
    Camera( double w, double h ) : width( w ), height( h )
    {
    }

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
    Vec3 center{};
    double radius{};

    Sphere() = default;
    Sphere( Vec3 c, double r ) : center( std::move( c ) ), radius( r )
    {
    }

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
    Pixel bgColor{ 1, 1, 1 };

    void add( IHitable* ptr )
    {
        hitableList.add( ptr );
    }

    [[nodiscard]] Pixel generateBackgroundColor( const Ray& ray ) const
    {
        return bgColor;
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
                return render( scatterRecord.ray ) * scatterRecord.attenuation;
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

    std::mt19937 eng{ std::random_device()() };
    std::uniform_real_distribution< double > dist{ 0, 1 };

    ImageDriver() = default;
    ImageDriver( int x, int y, int subs )
        : xNumPixels( x ), yNumPixels( y ), subSamples( subs )
    {
    }

    inline double randomOffset()
    {
        return dist( eng );
    }

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
                    double u = double( x + randomOffset() ) / double( xNumPixels );
                    double v = double( y + randomOffset() ) / double( yNumPixels );
                    pixel = pixel + ren.render( cam.getRay( u, v ) );
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

using OptError = std::optional< std::string >;

OptError ensure_it_compiles()
{
    std::ostringstream oss;
    ImageDriver id;
    Sphere sphere{};
    Renderer renderer{};
    renderer.add( &sphere );
    if ( auto status = id.drive( Camera{}, renderer ); status )
    {
        std::string expected{ R"(P3
4 4
255
255 255 255 255 255 255 255 255 255 255 255 255
255 255 255 255 255 255 255 255 255 255 255 255
255 255 255 255 255 255 255 255 255 255 255 255
255 255 255 255 255 255 255 255 255 255 255 255
)" };
        id.output( oss );
        assert( expected == oss.str() );
        return std::nullopt;
    }
    else
    {
        return "failed to render dummy image to text";
    }
}

OptError ensure_it_generate_background_color()
{
    ImageDriver id{ 300, 200, 16 };  // 3 : 2
    Sphere sphere{};
    Renderer renderer{};
    renderer.add( &sphere );
    if ( auto status = id.drive( Camera{ 3.0, 2.0 }, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/out.ppm" };
        id.output( ofs );
        return std::nullopt;
    }
    else
    {
        return "failed to render background color to file";
    }
}

int main()
{
    if ( auto err = ensure_it_compiles(); err )
    {
        assert( false );
    }
    if ( auto err = ensure_it_generate_background_color(); err )
    {
        assert( false );
    }
    return 0;
}