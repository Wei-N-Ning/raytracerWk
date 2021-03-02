//
// Created by wein on 2/09/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <material.hh>
#include <simpleRender.hh>

#include <limits>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>

TEST_CASE( "lambert material" )
{
    std::cout << "lambert test" << '\n';

    using namespace RTWK1;

    std::ofstream ofs;
    ofs.open( "/tmp/lambert.ppm" );
    CHECK( ofs.good() );
    RTWK1::Camera camera;

    RTWK1::SimpleRender render;
    render.m_materials[ "lambert.grey" ] = new Lambertian( { 0.5f, 0.5f, 0.5f } );

    render.m_entities.push_back(
        new Sphere( Vec3( 0, 0, -1 ), 0.5, render.m_materials.at( "lambert.grey" ) ) );
    render.m_entities.push_back( new Sphere(
        Vec3( 0, -100.5f, -1 ), 100, render.m_materials.at( "lambert.grey" ) ) );
    render.m_entities.push_back(
        new Sphere( Vec3( 1, 0, -1 ), 0.5, render.m_materials.at( "lambert.grey" ) ) );
    render.m_entities.push_back(
        new Sphere( Vec3( -1, 0, -1 ), 0.5, render.m_materials.at( "lambert.grey" ) ) );

    render.updateWorld();

    RTWK1::createImageMaterial( ofs, 200, 100, 32, camera, render, true );
}

void metal( float fuzziness, const char* filename )
{
    using namespace RTWK1;

    std::ofstream ofs;
    ofs.open( filename );
    CHECK( ofs.good() );
    RTWK1::Camera camera;

    SimpleRender render;
    render.m_materials[ "lambert.red" ] = new Lambertian( { 0.8f, 0.3f, 0.3f } );
    render.m_materials[ "lambert.orange" ] = new Lambertian( { 0.8f, 0.8f, 0 } );
    render.m_materials[ "metal.green" ] = new Metal( { 0.8f, 0.6f, 0.2f }, fuzziness );
    render.m_materials[ "metal.lightgrey" ] =
        new Metal( { 0.8f, 0.8f, 0.8f }, fuzziness );

    render.m_entities.push_back(
        new Sphere( Vec3( 0, 0, -1 ), 0.5, render.m_materials.at( "lambert.red" ) ) );
    render.m_entities.push_back( new Sphere(
        Vec3( 0, -100.5f, -1 ), 100, render.m_materials.at( "lambert.orange" ) ) );
    render.m_entities.push_back(
        new Sphere( Vec3( 1, 0, -1 ), 0.5, render.m_materials.at( "metal.green" ) ) );
    render.m_entities.push_back( new Sphere(
        Vec3( -1, 0, -1 ), 0.5, render.m_materials.at( "metal.lightgrey" ) ) );

    render.updateWorld();

    RTWK1::createImageMaterial( ofs, 200, 100, 32, camera, render, true );
}

TEST_CASE( "metal material: fuzziness 0 " )
{
    metal( 0, "/tmp/metal_f0.ppm" );
}

TEST_CASE( "metal material: fuzziness 0.25" )
{
    metal( 0.25f, "/tmp/metal_f025.ppm" );
}
