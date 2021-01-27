//
// Created by weining on 6/1/21.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <array>
#include <algorithm>
#include <execution>
#include <functional>
#include <utility>
#include <cmath>
#include <iostream>
#include <sstream>
#include <optional>
#include <memory>
#include <random>

using Vec3 = std::array< double, 3 >;

template < typename ElementOp >
inline Vec3 binaryOp( const Vec3& a, const Vec3& b ) noexcept
{
    ElementOp op{};
    return Vec3{ op( a[ 0 ], b[ 0 ] ), op( a[ 1 ], b[ 1 ] ), op( a[ 2 ], b[ 2 ] ) };
}

template < typename ElementOp >
inline Vec3 binaryOp( const Vec3& a, double b ) noexcept
{
    ElementOp op{};
    return Vec3{ op( a[ 0 ], b ), op( a[ 1 ], b ), op( a[ 2 ], b ) };
}

inline Vec3 operator+( const Vec3& a, const Vec3& b ) noexcept
{
    return binaryOp< std::plus< double > >( a, b );
}

inline Vec3 operator-( const Vec3& a, const Vec3& b ) noexcept
{
    return binaryOp< std::minus< double > >( a, b );
}

// this is the dot product
inline double operator*( const Vec3& a, const Vec3& b ) noexcept
{
    return a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ 1 ] + a[ 2 ] * b[ 2 ];
}

inline Vec3 operator*( const Vec3& a, double b ) noexcept
{
    return binaryOp< std::multiplies< double > >( a, b );
}

inline Vec3 operator/( const Vec3& a, double b ) noexcept
{
    return binaryOp< std::divides< double > >( a, b );
}

inline Vec3 cross( const Vec3& a, const Vec3& b ) noexcept
{
    return Vec3{ a[ 1 ] * b[ 2 ] - a[ 2 ] * b[ 1 ],
                 a[ 2 ] * b[ 0 ] - a[ 0 ] * b[ 2 ],
                 a[ 0 ] * b[ 1 ] - a[ 1 ] * b[ 0 ] };
}

inline double length( const Vec3& v ) noexcept
{
    return std::sqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );
}

inline Vec3 normalized( const Vec3& v ) noexcept
{
    auto l = length( v );
    return Vec3{ v[ 0 ] / l, v[ 1 ] / l, v[ 2 ] / l };
}

// ray(t) = A + B * t
using Ray = std::pair< Vec3, Vec3 >;

inline Vec3 origin( const Ray& r ) noexcept
{
    return r.first;
}

inline Vec3 direction( const Ray& r ) noexcept
{
    return r.second;
}

inline Vec3 at( const Ray& r, double t ) noexcept
{
    return r.first + r.second * t;
}

TEST_CASE( "vector algebra" )
{
    // length
    {
        CHECK_EQ( 1, length( Vec3{ 0, 0, 1 } ) );
    }
    // normalized
    {
        CHECK_EQ( Vec3{ 1, 0, 0 }, normalized( Vec3{ 1000, 0, 0 } ) );
    }
    // addition
    {
        CHECK_EQ( Vec3{ 1, 2, 3 } + Vec3{ 2, 3, 4 }, Vec3{ 3, 5, 7 } );
    }
    // subtraction
    {
        CHECK_EQ( Vec3{ 1, 2, 3 } - Vec3{ 2, 3, 4 }, Vec3{ -1, -1, -1 } );
    }
    // scale
    {
        CHECK_EQ( Vec3{ 1, 2, 3 } * 3, Vec3{ 3, 6, 9 } );
        CHECK_EQ( Vec3{ 3, 6, 9 } / 3, Vec3{ 1, 2, 3 } );
    }
    // dot product
    {
        CHECK_EQ( Vec3{ 1, 2, 3 } * Vec3{ 1, 2, 3 }, 14 );
    }
    // cross product
    {
        Vec3 o = cross( Vec3{ 0, 0, -1 }, Vec3{ -1, 0, 0 } );
        CHECK_EQ( o, Vec3{ 0, 1, 0 } );
    }
}

struct Viewport
{
    std::vector< Vec3 > pixels{};
    int xNumPixels{};
    int yNumPixels{};
    double w{};
    double h{};
    double halfW{};
    double halfH{};

    Viewport( int xPixels, int yPixels, double width, double height )
        : pixels( xPixels * yPixels )
        , xNumPixels( xPixels )
        , yNumPixels( yPixels )
        , w( width )
        , h( height )
        , halfW( width / 2.0 )
        , halfH( height / 2.0 )
    {
    }

    inline void foreach ( const std::function< Vec3( const Ray& ) >& f )
    {
        static std::mt19937 eng{ std::random_device()() };
        static std::uniform_real_distribution< double > dist{ -1, 1 };

        Vec3 lowerLeftCorner{ -w / 2, -h / 2, -1.0 };
        Vec3 horizontal{ w, 0.0, 0.0 };
        Vec3 vertical{ 0.0, h, 0.0 };
        Vec3 origin{};

        size_t idx = 0;
        size_t numSamples = 32;
        for ( auto y = yNumPixels - 1; y >= 0; --y )
        {
            for ( auto x = 0; x < xNumPixels; ++x )
            {
                Vec3 color{};
                for ( auto s = 0; s < numSamples; ++s )
                {
                    double u = double( x + dist( eng ) ) / double( xNumPixels );
                    double v = double( y + dist( eng ) ) / double( yNumPixels );

                    color = color
                            + f( Ray{ origin,
                                      lowerLeftCorner + horizontal * u + vertical * v } );
                }

                pixels[ idx++ ] = color / double( numSamples );
            }
        }
    }

    std::ostream& write( std::ostream& os ) const
    {
        os << "P3\n" << xNumPixels << " " << yNumPixels << "\n255\n";
        std::for_each( std::cbegin( pixels ),
                       std::cend( pixels ),
                       [ w = xNumPixels, &os, idx = 1 ]( const Vec3& pix ) mutable {
                           os << ( int )( pix[ 0 ] ) << " " << ( int )( pix[ 1 ] ) << " "
                              << ( int )( pix[ 2 ] ) << ( idx++ % w == 0 ? '\n' : ' ' );
                       } );
        return os;
    }
};

TEST_CASE( "test image io" )
{
    std::ostringstream oss;
    Viewport v{ 2, 2, 1.0, 1.0 };
    v.write( oss );
    CHECK_EQ( std::string{ R"(P3
2 2
255
0 0 0 0 0 0
0 0 0 0 0 0
)" },
              oss.str() );
}

using HitRecord = std::pair< Vec3, Vec3 >;  // color, hitPoint, normal

struct IHitable
{
    virtual std::optional< HitRecord > hitTest( const Ray& in,
                                                double t_min,
                                                double t_max ) = 0;

    virtual std::optional< Ray > reflect( const Ray& in, const Vec3& point )
    {
        return std::nullopt;
    };
};

struct GradientBackground : IHitable
{
    // [A]bove
    Vec3 toneA{ 1.0, 1.0, 1.0 };
    // [B]ottom
    Vec3 toneB{ 0.5, 0.7, 1.0 };

    GradientBackground() = default;

    GradientBackground( const Vec3& a, const Vec3& b ) : toneA( a ), toneB( b )
    {
    }

    std::optional< HitRecord > hitTest( const Ray& in, double, double ) override
    {
        auto ratio = ( normalized( direction( in ) )[ 1 ] + 1.0 ) * 0.5;
        return std::make_pair( toneA * ( 1.0 - ratio ) + toneB * ratio, Vec3{} );
    }
};

TEST_CASE( "hit-test background" )
{
    Ray r{ { 0, 0, 0 }, { 0.1, 0.3, -1 } };
    GradientBackground bg{};
    // always hit
    CHECK( bg.hitTest( r, 0, 0 ) );
    {
        // test image
        std::ostringstream oss;
        Viewport v{ 32, 24, 4.0, 3.0 };
        v.foreach (
            //
            [ bg = GradientBackground{} ]( const auto& ray ) mutable -> Vec3 {
                if ( auto optRecord = bg.hitTest( ray, 0, 0 ); optRecord )
                {
                    return optRecord->first;
                }
                return {};
            } );
        v.write( oss );
    }
}

Vec3 randomInUnitSphere()
{
    static std::mt19937 eng{ std::random_device()() };
    static std::uniform_real_distribution< double > dist{ -1, 1 };
    Vec3 p{};
    Vec3 aim{ 1, 1, 1 };
    do
    {
        p = Vec3{ dist( eng ), dist( eng ), dist( eng ) } - aim;
    } while ( p * p >= 1.0 );
    return p;
}

struct Sphere : IHitable
{
    Vec3 center{};
    double radius{};
    Vec3 color{ 150, 100, 100 };

    Sphere( const Vec3& v, double r ) : center( v ), radius( r )
    {
    }

    Sphere( const Vec3& v, double r, Vec3 col ) : center( v ), radius( r ), color( col )
    {
    }

    std::optional< HitRecord > hitTest( const Ray& in,
                                        double t_min,
                                        double t_max ) override
    {
        Vec3 c_P = origin( in ) - center;  // origin <- center
        Vec3 dir = direction( in );
        double a = dir * dir;
        double b = 2.0 * ( c_P * dir );
        double c = c_P * c_P - radius * radius;
        double discriminant = b * b - 4 * a * c;
        if ( discriminant < 0 )
        {
            // no hit, discard this ray
            return std::nullopt;
        }

        auto t = ( -b - std::sqrt( discriminant ) ) / ( 2.0 * a );
        if ( t < 0.0 )
        {
            return std::nullopt;
        }

        Vec3 N = normalized( at( in, t ) - center );  // at() <- center
        Vec3 col{
            std::get< 0 >( N ) + 1, std::get< 1 >( N ) + 1, std::get< 2 >( N ) + 1 };
        return std::make_pair( col * 0.5, Vec3{} );
    }

    std::optional< Ray > reflect( const Ray& in, const Vec3& point ) override
    {
        auto v = normalized( direction( in ) );
        auto N = normalized( point - center );
        return Ray{ point, normalized( v - N * ( v * N ) * 2 ) };
    }
};

TEST_CASE( "hit-test sphere" )
{
    Sphere s( { 2, 2, 2 }, 1 );
    // hit
    {
        Ray r1{ { 0.1, 0.1, 0.1 }, { 2.1, 2.1, 2.1 } };
        auto hit = s.hitTest( r1, 0, 9999 );
        CHECK( hit );
    }  // discard
    {
        Ray r1{ { 0.1, 0.1, 0.1 }, { -2.1, -2.1, -2.1 } };
        auto hit = s.hitTest( r1, 0, 9999 );
        CHECK_FALSE( hit );
    }
}

using HitableCollection = std::vector< std::shared_ptr< IHitable > >;

auto hitTest( const HitableCollection& coll, const Ray& in, double t_min, double t_max )
    -> std::optional< HitRecord >
{
    for ( const auto& hitable : coll )
    {
        if ( auto optRecord = hitable->hitTest( in, 0, 1 ); optRecord )
        {
            // const auto& [ color, _ ] = *optRecord;
            return optRecord;
        }
    }
    return std::nullopt;
}

TEST_CASE( "render" )
{
    HitableCollection hitables{ std::make_shared< Sphere >( Vec3{ 0, 0, -1 }, 0.5 ),
                                std::make_shared< Sphere >( Vec3{ 0, -100.5, -1 }, 100 ),
                                std::make_shared< GradientBackground >() };
    std::function< Vec3( const Ray&, size_t ) > render
        //
        = [ & ]( const Ray& ray, size_t ) -> Vec3 {
        if ( auto optRecord = hitTest( hitables, ray, 0, 1 ); optRecord )
        {
            const auto& [ color, _ ] = *optRecord;
            return Vec3{ 255 * std::get< 0 >( color ),
                         255 * std::get< 1 >( color ),
                         255 * std::get< 2 >( color ) };
        }
        return {};
    };
    std::ofstream ofs{ "/home/weining/work/tmp/out.ppm" };
    Viewport v{ 200, 100, 4.0, 2.0 };
    v.foreach (
        //
        [ & ]( const auto& ray ) mutable -> Vec3 { return render( ray, 3 ); } );
    v.write( ofs );
}