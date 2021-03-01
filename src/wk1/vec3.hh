//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_VEC3_HH
#define RAYTRACER_IN_A_WEEKEND_VEC3_HH

#include <istream>
#include <ostream>
#include <vector>
#include <cmath>

namespace RTWK1
{
struct Vec3
{
    Vec3();

    Vec3( float e0, float e1, float e2 );

    float x() const;

    float y() const;

    float z() const;

    float r() const;

    float g() const;

    float b() const;

    const Vec3& operator+() const;

    Vec3 operator-() const;

    float operator[]( int i ) const;

    float& operator[]( int i );

    Vec3& operator+=( const Vec3& other );

    Vec3& operator-=( const Vec3& other );

    Vec3& operator*=( const Vec3& other );

    Vec3& operator*=( float v );

    Vec3& operator/=( const Vec3& other );

    Vec3& operator/=( float v );

    float length() const;

    float sqr_length() const;

    static Vec3 unit();
    Vec3 normalized() const;

    std::vector< float > m_elements;
};

std::istream& operator>>( std::istream& is, Vec3& v );
std::ostream& operator<<( std::ostream& os, const Vec3& v );

Vec3 operator+( const Vec3& lhs, const Vec3& rhs );
Vec3 operator*( const Vec3& lhs, const Vec3& rhs );
Vec3 operator-( const Vec3& lhs, const Vec3& rhs );
Vec3 operator/( const Vec3& lhs, const Vec3& rhs );
Vec3 operator*( float v, const Vec3& rhs );
Vec3 operator*( const Vec3& lhs, float v );
Vec3 operator/( const Vec3& lhs, float v );

float dot( const Vec3& lhs, const Vec3& rhs );
Vec3 cross( const Vec3& lhs, const Vec3& rhs );

bool operator==( const Vec3& lhs, const Vec3& rhs );

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_VEC3_HH
