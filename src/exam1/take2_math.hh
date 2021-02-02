//
// Created by weining on 2/2/21.
//

#ifndef RAYTRACERWEEKEND_TAKE2_MATH_HH
#define RAYTRACERWEEKEND_TAKE2_MATH_HH

#include <tuple>
#include <functional>
#include <cmath>
#include <ostream>

using Vec3 = std::tuple< double, double, double >;

using Pixel = Vec3;

std::ostream& operator<<( std::ostream& os, const Pixel& pix )
{
    const auto& [ r, g, b ] = pix;
    auto ri = int( 255.99 * r );
    auto gi = int( 255.99 * g );
    auto bi = int( 255.99 * b );
    return os << ri << ' ' << gi << ' ' << bi;
}

template < typename ElementOp >
inline Vec3 binaryOp( const Vec3& a, const Vec3& b ) noexcept
{
    ElementOp op{};
    const auto& [ a1, a2, a3 ] = a;
    const auto& [ b1, b2, b3 ] = b;
    return Vec3{ op( a1, b1 ), op( a2, b2 ), op( a3, b3 ) };
}

template < typename ElementOp >
inline Vec3 binaryOp( const Vec3& a, double b ) noexcept
{
    ElementOp op{};
    const auto& [ a1, a2, a3 ] = a;
    return Vec3{ op( a1, b ), op( a2, b ), op( a3, b ) };
}

inline Vec3 operator+( const Vec3& a, const Vec3& b ) noexcept
{
    return binaryOp< std::plus< double > >( a, b );
}

inline Vec3 operator-( const Vec3& a, const Vec3& b ) noexcept
{
    return binaryOp< std::minus< double > >( a, b );
}

inline Vec3 operator*( const Vec3& a, const Vec3& b ) noexcept
{
    return binaryOp< std::multiplies< double > >( a, b );
}

inline Vec3 operator/( const Vec3& a, const Vec3& b ) noexcept
{
    return binaryOp< std::divides< double > >( a, b );
}

inline Vec3 operator*( const Vec3& a, double b ) noexcept
{
    return binaryOp< std::multiplies< double > >( a, b );
}

inline Vec3 operator/( const Vec3& a, double b ) noexcept
{
    return binaryOp< std::divides< double > >( a, b );
}

inline double dot( const Vec3& a, const Vec3& b ) noexcept
{
    const auto& [ a1, a2, a3 ] = a;
    const auto& [ b1, b2, b3 ] = b;
    return a1 * b1 + a2 * b2 + a3 * b3;
}

inline Vec3 cross( const Vec3& a, const Vec3& b ) noexcept
{
    const auto& [ a0, a1, a2 ] = a;
    const auto& [ b0, b1, b2 ] = b;
    return Vec3{ a1 * b2 - a2 * b1, a2 * b0 - a0 * b2, a0 * b1 - a1 * b0 };
}

inline double length( const Vec3& v ) noexcept
{
    const auto& [ v0, v1, v2 ] = v;
    return std::sqrt( v0 * v0 + v1 * v1 + v2 * v2 );
}

inline Vec3 normalized( const Vec3& v ) noexcept
{
    auto l = length( v );
    const auto& [ v0, v1, v2 ] = v;
    return Vec3{ v0 / l, v1 / l, v2 / l };
}

// ray(t) = A + B * t
using Ray = std::pair< Vec3, Vec3 >;

// use structured binding to access origin and direction:
// auto &[orig, dir] = ray;

inline Vec3 at( const Ray& r, double t ) noexcept
{
    return r.first + r.second * t;
}

#endif  // RAYTRACERWEEKEND_TAKE2_MATH_HH
