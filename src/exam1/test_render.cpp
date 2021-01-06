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
struct Ray : public std::pair< Vec3, Vec3 >
{
    inline Vec3& origin() noexcept
    {
        return this->first;
    }
    inline Vec3& direction() noexcept
    {
        return this->second;
    }
};

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
    size_t width{};
    size_t height{};

    Viewport( size_t w, size_t h ) : pixels( w * h ), width( w ), height( h )
    {
    }

    inline void foreach ( std::function< Vec3( size_t, size_t ) >& f )
    {
        for ( auto y = 0; y < height; ++y )
        {
            for ( auto x = 0; x < width; ++x )
            {
                pixels[ y * width + x ] = f( x, y );
            }
        }
    }

    std::ostream& write( std::ostream& os ) const
    {
        os << "P3\n" << width << " " << height << "\n255\n";
        std::for_each( std::cbegin( pixels ),
                       std::cend( pixels ),
                       [ w = width, &os, idx = 1 ]( const Vec3& pix ) mutable {
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
    CHECK_EQ(std::string{R"(P3
2 2
255
0 0 0 0 0 0
0 0 0 0 0 0
)"}, oss.str());
}