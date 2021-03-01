//
// Created by wein on 26/08/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <vec3.hh>

#include <sstream>
#include <tuple>

using F3 = std::tuple< float, float, float >;

inline F3 f3( const RTWK1::Vec3& v )
{
    return { v[ 0 ], v[ 1 ], v[ 2 ] };
}

TEST_CASE( "test vector ctor" )
{
    using namespace RTWK1;

    Vec3 v1{ 1, 2, 3 };
    Vec3 v2{ ( -v1 )[ 2 ], ( +v1 ).g(), ( -v1 ).x() };
    CHECK_EQ( f3( v2 ), F3{ -3, 2, -1 } );
}

TEST_CASE( "change sign" )
{
    using namespace RTWK1;

    Vec3 v1{ 1, 2, 3 };
    Vec3 v2{ ( -v1 )[ 2 ], ( +v1 ).g(), ( -v1 ).x() };

    v2 += v1;
    CHECK_EQ( f3( v2 ), F3{ -2, 4, 2 } );

    v1 -= v2;
    CHECK_EQ( f3( v1 ), F3{ 3, -2, 1 } );
}

TEST_CASE( "vector arithmetics" )
{
    using namespace RTWK1;

    Vec3 v1{ 1, 2, 3 };

    v1 *= v1;
    CHECK_EQ( f3( v1 ), F3{ 1, 4, 9 } );

    v1 /= v1;
    CHECK_EQ( f3( v1 ), F3{ 1, 1, 1 } );
    CHECK_EQ( f3( Vec3::unit() ), f3(v1) );

    Vec3 v3{ 10, 5, 15 };
    CHECK_EQ( f3( v3 *= 0.2 ), F3{ 2, 1, 3 } );
    CHECK_EQ( f3( v3 /= 0.2 ), F3{ 10, 5, 15 } );
}

TEST_CASE( "vector length" )
{
    using namespace RTWK1;

    Vec3 v3{ 10, 5, 15 };
    CHECK_EQ( 12, Vec3( 2, 2, 2 ).sqr_length() );
    CHECK_EQ( 2, Vec3( 2, 0, 0 ).length() );
}

TEST_CASE( "vector ostream operator" )
{
    using namespace RTWK1;

    Vec3 v3{ 10, 5, 15 };
    std::stringstream ss;
    ss << v3;

    CHECK_EQ( "10 5 15", ss.str() );
}

TEST_CASE( "vector istream operator" )
{
    using namespace RTWK1;

    Vec3 tmp;
    std::stringstream ss;
    ss.str( std::string{ "11 22 33\n44 55 66" } );
    ss >> tmp;
    CHECK_EQ( f3( tmp ), F3{ 11, 22, 33 } );
    ss >> tmp;
    CHECK_EQ( f3( tmp ), F3{ 44, 55, 66 } );
    ss >> tmp;
    CHECK_EQ( f3( tmp ), F3{ 44, 55, 66 } );
}

TEST_CASE( "dot product" )
{
    using namespace RTWK1;

    CHECK_EQ( -14, dot( { -1, 2, -3 }, { 1, -2, 3 } ) );
}

TEST_CASE( "cross product" )
{
    using namespace RTWK1;

    Vec3 result = cross( { 1, 0, 0 }, { 0, 1, 0 } );
    CHECK_EQ( F3{ 0, 0, 1 }, f3( result ) );

    result = cross( { 0, 1, 0 }, { 1, 0, 0 } );
    CHECK_EQ( F3{ 0, 0, -1 }, f3( result ) );
}

TEST_CASE( "normalize" )
{
    using namespace RTWK1;

    CHECK_EQ( F3{ 1, 0, 0 }, f3( Vec3{ 10, 0, 0 }.normalized() ) );
}
