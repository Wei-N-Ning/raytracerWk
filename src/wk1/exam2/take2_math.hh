//
// Created by weining on 2/2/21.
//

#ifndef RAYTRACERWEEKEND_TAKE2_MATH_HH
#define RAYTRACERWEEKEND_TAKE2_MATH_HH

#include <tuple>
#include <functional>
#include <cmath>
#include <ostream>
#include <optional>

using Vec3 = std::tuple< double, double, double >;

using Color = Vec3;

std::ostream& operator<<( std::ostream& os, const Color& pix )
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

inline Vec3 operator-( const Vec3& v ) noexcept
{
    const auto& [ v0, v1, v2 ] = v;
    return Vec3{ -v0, -v1, -v2 };
}

inline double length( const Vec3& v ) noexcept
{
    const auto& [ v0, v1, v2 ] = v;
    return std::sqrt( v0 * v0 + v1 * v1 + v2 * v2 );
}

inline double squareLength( const Vec3& v ) noexcept
{
    const auto& [ v0, v1, v2 ] = v;
    return v0 * v0 + v1 * v1 + v2 * v2;
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

inline Vec3 reflect( const Vec3& v, const Vec3& n )
{
    // G.intro 5th P/498
    // projection matrix: (I - 2 n nT)
    // to project a vector v, do: v (I - 2 n nT) = v - 2 n (nT v) = v - 2 n (n . v)
    return v - n * dot( v, n ) * 2;
}

// ni_over_nt is the refractive index
// Snell's law
inline std::optional< Vec3 > refract( const Vec3& v, const Vec3& n, double ni_over_nt )
{
    Vec3 uv = normalized( v );
    double dt = dot( uv, n );
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * ( 1 - dt * dt );
    if ( discriminant > 0 )
    {
        return ( uv - n * dt ) * ni_over_nt - n * std::sqrt( discriminant );
    }
    return std::nullopt;
}

inline double schlick( double cosine, double ref_idx )
{
    double r0 = ( 1 - ref_idx ) / ( 1 + ref_idx );
    r0 = r0 * r0;
    return r0 + ( 1 - r0 ) * std::pow( ( 1 - cosine ), 5 );
}

inline std::optional< std::tuple< double, double, double > > solveQuadratic(
    double a,
    double b,
    double c ) noexcept
{
    double discriminant = b * b - a * c * 4;
    if ( discriminant < 0 )
    {
        return std::nullopt;
    }
    if ( discriminant == 0 )
    {
        double x = -b / a;
        return std::make_tuple( discriminant, x, x );
    }
    double s = std::sqrt( discriminant );
    double x0 = ( -b - s ) / 2.0;
    double x1 = ( -b + s ) / 2.0;
    return std::make_tuple( discriminant, x0, x1 );
}

// source:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
inline bool intersect( const Ray& r, const Vec3& position, double radius ) noexcept
{
    const auto& [ O, D ] = r;
    auto OC = O - position;
    /*
     * (P - Pos)^2 - R^2 = 0
     * P^2 - 2 P Pos + Pos^2 - R^2 = 0
     * (O + D t)^2 - 2(O + D t) Pos + (Pos^2 - R^2) = 0
     * D^2 t^2 + 2 O D t - 2 Pos D t + (O^2 - 2 O Pos + Pos^2) - R^2 = 0
     * D^2 t^2 + 2 (O D - Pos D) t + |O - Pos|^2 - R^2 = 0
     *             ^^^^^^^^^^^^ D (O - Pos) !!
     * because D (direction vector) is normalized by convention, this equation can be
     * simplified to:
     *
     * t^2 + 2 (O - Pos) t + |O - Pos|^2 - R^2 = 0
     *
     * but if not taking the simplified version:
     * a = dot(D, D)
     * b = dot(D, OC) * 2 (see notes above)
     * c = dot(OC, OC) - R * R
     *
     * */

    auto a = dot( D, D );
    auto b = dot( D, OC ) * 2;
    auto c = dot( OC, OC ) - radius * radius;
    return solveQuadratic( a, b, c ).has_value();
}

#endif  // RAYTRACERWEEKEND_TAKE2_MATH_HH
