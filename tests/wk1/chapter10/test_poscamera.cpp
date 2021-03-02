//
// Created by wein on 6/10/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <simpleRender.hh>
#include <material.hh>

#include <fstream>

inline RTWK1::SimpleRender createScene()
{
    using namespace RTWK1;

    SimpleRender render;
    render.m_materials[ "lambert.red" ] = new Lambertian( { 1.0f, 0.1f, 0.0f } );
    render.m_materials[ "lambert.blue" ] = new Lambertian( { 0.0f, 0.3f, 1.0f } );
    render.m_materials[ "lambert.grey" ] = new Lambertian( { 0.3f, 0.3f, 0.4f } );

    render.m_entities.push_back( new Sphere(
        Vec3( -0.4f, 0, -1 ), 0.4f, render.m_materials.at( "lambert.blue" ) ) );
    render.m_entities.push_back(
        new Sphere( Vec3( 0.8f, 0, -1 ), 0.8f, render.m_materials.at( "lambert.red" ) ) );
    render.m_entities.push_back( new Sphere(
        Vec3( 0, -100.5f, -1 ), 100, render.m_materials.at( "lambert.grey" ) ) );

    render.updateWorld();

    return render;
}

TEST_CASE( "test_camera_fov" )
{
    using namespace RTWK1;

    SimpleRender render = createScene();

    RTWK1::Camera camera;
    std::ofstream ofs;

    camera = Camera( 45.0f, 2.0f );
    ofs.open( "/tmp/test_camera_fov45.ppm" );
    RTWK1::createImageMaterial( ofs, 200, 100, 8, camera, render, true );
    ofs.close();

    camera = Camera( 90.0f, 2.0f );
    ofs.open( "/tmp/test_camera_fov90.ppm" );
    RTWK1::createImageMaterial( ofs, 200, 100, 8, camera, render, true );
    ofs.close();

    camera = Camera( 120.0f, 2.0f );
    ofs.open( "/tmp/test_camera_fov120.ppm" );
    RTWK1::createImageMaterial( ofs, 200, 100, 8, camera, render, true );
    ofs.close();
}

TEST_CASE( "test_camera_orientation" )
{
    using namespace RTWK1;

    SimpleRender render = createScene();

    RTWK1::Camera camera;
    std::ofstream ofs;

    camera = Camera(
        Vec3( -1.35f, 1.45f, 0.7f ), Vec3( 0, 0, -1 ), Vec3( 0, 1, 0 ), 60.0f, 2.0f );
    ofs.open( "/tmp/test_camera_.ppm" );
    RTWK1::createImageMaterial( ofs, 200, 100, 16, camera, render, true );
    ofs.close();
}
