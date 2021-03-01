//
// Created by wein on 26/08/18.
//

#include "vec3.hh"

namespace RTWK
{
Vec3::Vec3()
{
    m_elements.assign( { 0, 0, 0 } );
}

Vec3::Vec3( float e0, float e1, float e2 )
{
    m_elements.assign( { e0, e1, e2 } );
}

float Vec3::x() const
{
    return m_elements[ 0 ];
}

float Vec3::y() const
{
    return m_elements[ 1 ];
}

float Vec3::z() const
{
    return m_elements[ 2 ];
}

float Vec3::r() const
{
    return m_elements[ 0 ];
}

float Vec3::g() const
{
    return m_elements[ 1 ];
}

float Vec3::b() const
{
    return m_elements[ 2 ];
}

const Vec3 &Vec3::operator+() const
{
    return *this;
}

Vec3 Vec3::operator-() const
{
    return Vec3( -m_elements[ 0 ], -m_elements[ 1 ], -m_elements[ 2 ] );
}

float Vec3::operator[]( int i ) const
{
    return m_elements[ i ];
}

float &Vec3::operator[]( int i )
{
    return m_elements[ i ];
}

Vec3 &Vec3::operator+=( const Vec3 &other )
{
    m_elements[ 0 ] += other.m_elements[ 0 ];
    m_elements[ 1 ] += other.m_elements[ 1 ];
    m_elements[ 2 ] += other.m_elements[ 2 ];
    return *this;
}

Vec3 &Vec3::operator-=( const Vec3 &other )
{
    m_elements[ 0 ] -= other.m_elements[ 0 ];
    m_elements[ 1 ] -= other.m_elements[ 1 ];
    m_elements[ 2 ] -= other.m_elements[ 2 ];
    return *this;
}

Vec3 &Vec3::operator*=( const Vec3 &other )
{
    m_elements[ 0 ] *= other.m_elements[ 0 ];
    m_elements[ 1 ] *= other.m_elements[ 1 ];
    m_elements[ 2 ] *= other.m_elements[ 2 ];
    return *this;
}

Vec3 &Vec3::operator/=( const Vec3 &other )
{
    m_elements[ 0 ] /= other.m_elements[ 0 ];
    m_elements[ 1 ] /= other.m_elements[ 1 ];
    m_elements[ 2 ] /= other.m_elements[ 2 ];
    return *this;
}

Vec3 &Vec3::operator*=( float v )
{
    m_elements[ 0 ] *= v;
    m_elements[ 1 ] *= v;
    m_elements[ 2 ] *= v;
    return *this;
}

Vec3 &Vec3::operator/=( float v )
{
    m_elements[ 0 ] /= v;
    m_elements[ 1 ] /= v;
    m_elements[ 2 ] /= v;
    return *this;
}

float Vec3::length() const
{
    return std::sqrt( m_elements[ 0 ] * m_elements[ 0 ]
                      + m_elements[ 1 ] * m_elements[ 1 ]
                      + m_elements[ 2 ] * m_elements[ 2 ] );
}

float Vec3::sqr_length() const
{
    return m_elements[ 0 ] * m_elements[ 0 ] + m_elements[ 1 ] * m_elements[ 1 ]
           + m_elements[ 2 ] * m_elements[ 2 ];
}

Vec3 Vec3::unit()
{
    return { 1, 1, 1 };
}

std::istream &operator>>( std::istream &is, Vec3 &v )
{
    is >> v.m_elements[ 0 ] >> v.m_elements[ 1 ] >> v.m_elements[ 2 ];
    return is;
}

std::ostream &operator<<( std::ostream &os, const Vec3 &v )
{
    os << v.m_elements[ 0 ] << " " << v.m_elements[ 1 ] << " " << v.m_elements[ 2 ];
    return os;
}

Vec3 operator+( const Vec3 &lhs, const Vec3 &rhs )
{
    return Vec3( lhs.m_elements[ 0 ] + rhs.m_elements[ 0 ],
                 lhs.m_elements[ 1 ] + rhs.m_elements[ 1 ],
                 lhs.m_elements[ 2 ] + rhs.m_elements[ 2 ] );
}

Vec3 operator-( const Vec3 &lhs, const Vec3 &rhs )
{
    return Vec3( lhs.m_elements[ 0 ] - rhs.m_elements[ 0 ],
                 lhs.m_elements[ 1 ] - rhs.m_elements[ 1 ],
                 lhs.m_elements[ 2 ] - rhs.m_elements[ 2 ] );
}

Vec3 operator*( const Vec3 &lhs, const Vec3 &rhs )
{
    return Vec3( lhs.m_elements[ 0 ] * rhs.m_elements[ 0 ],
                 lhs.m_elements[ 1 ] * rhs.m_elements[ 1 ],
                 lhs.m_elements[ 2 ] * rhs.m_elements[ 2 ] );
}

Vec3 operator/( const Vec3 &lhs, const Vec3 &rhs )
{
    return Vec3( lhs.m_elements[ 0 ] / rhs.m_elements[ 0 ],
                 lhs.m_elements[ 1 ] / rhs.m_elements[ 1 ],
                 lhs.m_elements[ 2 ] / rhs.m_elements[ 2 ] );
}

Vec3 operator*( float v, const Vec3 &rhs )
{
    return Vec3(
        v * rhs.m_elements[ 0 ], v * rhs.m_elements[ 1 ], v * rhs.m_elements[ 2 ] );
}

Vec3 operator*( const Vec3 &lhs, float v )
{
    return Vec3(
        v * lhs.m_elements[ 0 ], v * lhs.m_elements[ 1 ], v * lhs.m_elements[ 2 ] );
}

Vec3 operator/( const Vec3 &lhs, float v )
{
    return Vec3(
        lhs.m_elements[ 0 ] / v, lhs.m_elements[ 1 ] / v, lhs.m_elements[ 2 ] / v );
}

float dot( const Vec3 &lhs, const Vec3 &rhs )
{
    return lhs.m_elements[ 0 ] * rhs.m_elements[ 0 ]
           + lhs.m_elements[ 1 ] * rhs.m_elements[ 1 ]
           + lhs.m_elements[ 2 ] * rhs.m_elements[ 2 ];
}

Vec3 cross( const Vec3 &lhs, const Vec3 &rhs )
{
    /*
     * | 1  1  1  |
     * | lx ly lz |
     * | rx ry rz |
     *
     * ly rz - lx rz + lz rx - ly rx + lx ry - lz ry
     * l1 r2 - l2 r1 + l2 r0 - l0 r2 + l0 r1 - l1 r0
     * */
    return Vec3( lhs.m_elements[ 1 ] * rhs.m_elements[ 2 ]
                     - lhs.m_elements[ 2 ] * rhs.m_elements[ 1 ],
                 lhs.m_elements[ 2 ] * rhs.m_elements[ 0 ]
                     - lhs.m_elements[ 0 ] * rhs.m_elements[ 2 ],
                 lhs.m_elements[ 0 ] * rhs.m_elements[ 1 ]
                     - lhs.m_elements[ 1 ] * rhs.m_elements[ 0 ] );
}

bool operator==( const Vec3 &lhs, const Vec3 &rhs )
{
    return lhs.m_elements[ 0 ] == rhs.m_elements[ 0 ]
           && lhs.m_elements[ 1 ] == rhs.m_elements[ 1 ]
           && lhs.m_elements[ 2 ] == rhs.m_elements[ 2 ];
}

Vec3 Vec3::normalized() const
{
    return *this / length();
}

}  // namespace RTWK
