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
    double width;
    double height;
    Vec3 position{ 0, 0, 0 };
    Vec3 lookAt{};
    Vec3 up{};
    Vec3 lowerLeftCorner{};
    Vec3 horizontal{};
    Vec3 vertical{};

    Camera( double w, double h )
        : width( w )
        , height( h )
        , lowerLeftCorner( w / -2.0, h / -2.0, -1.0 )
        , horizontal( w, 0, 0 )
        , vertical( 0, h, 0 )
    {
    }

    [[nodiscard]] Ray getRay( double u, double v ) const  // u, v is in the [0, 1] space
    {
        return Ray(
            //
            position,
            lowerLeftCorner + horizontal * u + vertical * v - position );
    }
};

struct HitRecord;

struct ScatterRecord
{
    Ray ray{};
    Vec3 attenuation{};
};

struct IMaterial
{
    virtual std::optional< ScatterRecord > scattered( const Ray& in,
                                                      const HitRecord& hitRecord ) = 0;
};

// the default, back hole material that does not scatter any ray
struct Material : public IMaterial
{
    std::optional< ScatterRecord > scattered( const Ray& in,
                                              const HitRecord& hitRecord ) override
    {
        return std::nullopt;
    }
};

struct HitRecord
{
    IMaterial* material;
    Vec3 normal{};
    Vec3 hitPoint{};
    double t{};
};

// NOTE: had a bug in the scattered() method
struct Lambertian : public IMaterial
{
    std::optional< ScatterRecord > scattered( const Ray& in,
                                              const HitRecord& hitRecord ) override
    {
        static std::mt19937 gen{ std::random_device()() };
        static std::uniform_real_distribution< double > dist( 0, 1 );

        Vec3 p{};
        do
        {
            Vec3 s = Vec3( dist( gen ), dist( gen ), dist( gen ) ) * 2;
            p = s - Vec3( 1, 1, 1 );
        } while ( squareLength( p ) >= 1.0 );

        Vec3 target = hitRecord.hitPoint + hitRecord.normal + p;
        Ray scatteredRay{ hitRecord.hitPoint, target - hitRecord.hitPoint };
        return ScatterRecord{ scatteredRay, { 0.5, 0.5, 0.5 } };
    }
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
    [[nodiscard]] virtual std::optional< HitRecord > hitTest(
        const Ray& ray,
        const RangeLimit& limit ) const = 0;
};

struct HitableList : public IHitable
{
    std::vector< IHitable* > pHitables{};

    void add( IHitable* ptr )
    {
        pHitables.push_back( ptr );
    }

    [[nodiscard]] std::optional< HitRecord > hitTest(
        const Ray& ray,
        const RangeLimit& limit ) const override
    {
        std::optional< HitRecord > ret{};
        RangeLimit li{ limit };
        for ( auto pHitable : pHitables )
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

// NOTE: had two major bugs in the hitTest() method - worth rewriting it with more unit
//       tests!
struct Sphere : public IHitable
{
    Vec3 center{};
    double radius{};
    IMaterial* mat{ nullptr };

    Sphere() = default;
    Sphere( Vec3 c, double r, IMaterial* material )
        : center( std::move( c ) ), radius( r ), mat( material )
    {
    }

    [[nodiscard]] std::optional< HitRecord > hitTest(
        const Ray& ray,
        const RangeLimit& limit ) const override
    {
        if ( !mat )
        {
            return std::nullopt;
        }
        const auto& [ orig, dir ] = ray;
        Vec3 oc = orig - center;
        double a = dot( dir, dir );
        double b = dot( oc, dir ) * 2;
        double c = dot( oc, oc ) - radius * radius;
        double discriminant = b * b - 4 * a * c;
        if ( discriminant < 0 )
        {
            return std::nullopt;
        }
        double t = ( -b - std::sqrt( discriminant ) ) / ( 2.0 * a );
        if ( t < limit.max && t > limit.min )
        {
            Vec3 hitPoint = at( ray, t );
            Vec3 surfaceNormal = normalized( hitPoint - center );
            return HitRecord{ //
                              mat,
                              surfaceNormal,
                              hitPoint,
                              t };
        }
        return std::nullopt;
    }
};

// use as the callable accepted by the renderer object
struct DualTone
{
    Color a{ 1, 1, 1 };
    Color b{ 1, 1, 1 };

    DualTone() = default;
    DualTone( Color c1, Color c2 ) : a( std::move( c1 ) ), b( std::move( c2 ) )
    {
    }

    [[nodiscard]] Color operator()( const Ray& ray ) const
    {
        const auto& [ _orig, dir ] = ray;
        auto dirN = normalized( dir );
        const auto& [ _x, y, _z ] = dirN;
        double t = ( y + 1.0 ) * 0.5;  // [0.0, 1.0]
        return b * ( 1 - t ) + a * t;
    }
};

struct Renderer
{
    HitableList hitableList{};
    std::function< Color( const Ray& ) > bgColorGenerator{ []( const Ray& ) -> Color {
        return { 1, 1, 1 };
    } };

    Renderer() = default;
    explicit Renderer( std::function< Color( const Ray& ) > f )
        : bgColorGenerator( std::move( f ) )
    {
    }

    void add( IHitable* ptr )
    {
        hitableList.add( ptr );
    }

    [[nodiscard]] Color render( const Ray& ray ) const
    {
        RangeLimit limit{ 0.000001, 10000000 };
        if ( auto optRecord = hitableList.hitTest( ray, limit ); optRecord )
        {
            auto& hitRecord = *optRecord;
            if ( auto optScaRecord = hitRecord.material->scattered( ray, hitRecord );
                 optScaRecord )
            {
                const auto& scatterRecord = *optScaRecord;
                return render( scatterRecord.ray ) * scatterRecord.attenuation;
            }
            else
            {
                return { 0, 0, 0 };
            }
        }
        return bgColorGenerator( ray );
    }
};

using Status = bool;

struct ImageDriver
{
    std::vector< Color > pixels{};
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
        size_t idx{ 0 };
        for ( auto y = yNumPixels - 1; y >= 0; --y )
        {
            for ( auto x = 0; x < xNumPixels; ++x )
            {
                Color pixel{};
                for ( auto ss = 0; ss < subSamples; ++ss )
                {
                    double u = double( x + randomOffset() ) / double( xNumPixels );
                    double v = double( y + randomOffset() ) / double( yNumPixels );
                    pixel = pixel + ren.render( cam.getRay( u, v ) );
                }
                pixels[ idx++ ] = postProcess( pixel / double( subSamples ) );
            }
        }
        return true;
    }

    [[nodiscard]] Color postProcess( Color pixel ) const
    {
        const auto& [ r, g, b ] = pixel;
        return Vec3( std::sqrt( r ), std::sqrt( g ), std::sqrt( b ) );
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
    Sphere sphere{ Vec3{ 99, 99, 99 }, 3, nullptr };
    Renderer renderer{};
    renderer.add( &sphere );
    if ( auto status = id.drive( Camera{ 1, 1 }, renderer ); status )
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
    ImageDriver id{ 100, 50, 2 };
    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    if ( auto status = id.drive( Camera{ 4.0, 2.0 }, renderer ); status )
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

OptError ensure_it_renders_single_sphere()
{
    ImageDriver id{ 300, 200, 64 };  // 3 : 2
    Lambertian diffuse{};
    Sphere sphere{ Vec3{ 0, 0, -1 }, 0.5, &diffuse };
    Sphere base{ Vec3{ 0, -100.5, -1 }, 100, &diffuse };
    Renderer renderer{ DualTone{ Color{ 0.5, 0.7, 1 }, Color{ 1, 1, 1 } } };
    renderer.add( &sphere );
    renderer.add( &base );
    if ( auto status = id.drive( Camera{ 3.0, 2.0 }, renderer ); status )
    {
        std::ofstream ofs{ "/tmp/1sphere.ppm" };
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
    if ( auto err = ensure_it_renders_single_sphere(); err )
    {
        assert( false );
    }
    return 0;
}