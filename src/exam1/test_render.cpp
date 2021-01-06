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
    double halfW{};
    double halfH{};

    Viewport( int w, int h )
        : pixels( w * h )
        , xNumPixels( w )
        , yNumPixels( h )
        , halfW( w / 2.0 )
        , halfH( h / 2.0 )
    {
    }

    inline void foreach ( const std::function< Vec3( double, double ) >& f )
    {
        static std::mt19937 eng{ std::random_device()() };
        static std::uniform_real_distribution< double > dist{ -0.01, 0.01 };
        for ( auto y = 0; y < yNumPixels; ++y )
        {
            for ( auto x = 0; x < xNumPixels; ++x )
            {
                // sub pixel sampling
                Vec3 color{};
                for ( auto i = 0; i < 8; ++i )
                {
                    color = color
                            + f( ( x - halfW ) / halfW + dist( eng ),
                                 ( y - halfH ) / halfH + dist( eng ) );
                }
                pixels[ y * xNumPixels + x ] = color / 8.0;
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
    Viewport v{ 2, 2 };
    v.write( oss );
    CHECK_EQ( std::string{ R"(P3
2 2
255
0 0 0 0 0 0
0 0 0 0 0 0
)" },
              oss.str() );
}

using HitRecord = std::pair< Vec3, Vec3 >;  // color, p

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
    Vec3 toneA{ 67, 127, 200 };
    Vec3 toneB{ 200, 248, 255 };

    GradientBackground() = default;

    GradientBackground( const Vec3& a, const Vec3& b ) : toneA( a ), toneB( b )
    {
    }

    std::optional< HitRecord > hitTest( const Ray& in, double, double ) override
    {
        auto ratio = normalized( direction( in ) )[ 1 ] / 1.0;
        return std::make_pair( toneA + ( toneB - toneA ) * ratio, Vec3{} );
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
        Viewport v{ 32, 24 };
        v.foreach (
            //
            [ bg = GradientBackground{} ]( auto x, auto y ) mutable -> Vec3 {
                if ( auto optRecord =
                         bg.hitTest( Ray{ { 0, 0, 0 }, { x, y, -3 } }, 0, 0 );
                     optRecord )
                {
                    return optRecord->first;
                }
                return {};
            } );
        v.write( oss );
    }
}

struct Sphere : IHitable
{
    Vec3 center{};
    double radius{};
    Vec3 color{ 150, 100, 100 };

    Sphere( const Vec3& v, double r ) : center( v ), radius( r )
    {
    }

    std::optional< HitRecord > hitTest( const Ray& in,
                                        double t_min,
                                        double t_max ) override
    {
        Vec3 c_P = origin( in ) - center;
        Vec3 dir = direction( in );
        double a = dir * dir;
        double b = 2.0f * ( c_P * dir );
        double c = c_P * c_P - radius * radius;
        double discriminant = b * b - 4 * a * c;
        if ( discriminant < 0 )
        {
            // no hit, discard this ray
            return std::nullopt;
        }
        double t = ( -b - std::sqrt( discriminant ) ) / ( 2.0f * a );
        if ( t < t_max && t > t_min )
        {
            // the near hit point t is within (t_min, t_max)
            return std::make_pair( color, at( in, t ) );
        }
        t = ( -b + std::sqrt( discriminant ) ) / ( 2.0f * a );
        if ( t < t_max && t > t_min )
        {
            // the near hit point t is outside (t_min, t_max);
            // find the second root to try to get the far hit point
            return std::make_pair( color, at( in, t ) );
        }
        return std::nullopt;
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

TEST_CASE( "render" )
{
    std::vector< std::shared_ptr< IHitable > > hitables{
        std::make_shared< Sphere >( Vec3{ 0, 0, -3 }, 0.1 ),
        std::make_shared< GradientBackground >() };
    {
        std::function< Vec3( const Ray&, size_t ) > render = [ & ]( const Ray& ray,
                                                                    size_t c ) -> Vec3 {
            for ( const auto& hitable : hitables )
            {
                if ( auto optRecord = hitable->hitTest( ray, 0, 1 ); optRecord )
                {
                    const auto& [ color, point ] = *optRecord;
                    if ( c > 0 )
                    {
                        if ( auto optReflected = hitable->reflect( ray, point );
                             optReflected )
                        {
                            return color * 0.25 + render( *optReflected, c - 1 ) * 0.75;
                        }
                    }
                    return color;
                }
            }
            return {};
        };
        std::ofstream ofs{ "/home/weining/work/tmp/out.ppm" };
        Viewport v{ 200, 100 };
        v.foreach (
            //
            [ & ]( auto x, auto y ) mutable -> Vec3 {
                Ray ray{ { 0, 0, 0 }, { x, y, -9 } };
                return render( ray, 3 );
            } );
        v.write( ofs );
    }
}