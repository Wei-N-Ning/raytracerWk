//
// Created by wein on 6/10/18.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <simpleRender.hh>
#include <material.hh>

#include <fstream>

RTWK1::SimpleRender createScene()
{
    using namespace RTWK1;

    SimpleRender render;
    render.m_materials[ "lambert.red" ] = new Lambertian( { 0.8f, 0.3f, 0.3f } );
    render.m_materials[ "lambert.orange" ] = new Lambertian( { 0.8f, 0.8f, 0 } );
    render.m_materials[ "metal.green" ] = new Metal( { 0.8f, 0.6f, 0.2f } );
    render.m_materials[ "metal.lightgrey" ] = new Metal( { 0.8f, 0.8f, 0.8f } );

    render.m_entities.push_back(
        new Sphere( Vec3( 0, 0, -1 ), 0.5, render.m_materials.at( "lambert.red" ) ) );
    render.m_entities.push_back( new Sphere(
        Vec3( 0, -100.5f, -1 ), 100, render.m_materials.at( "lambert.orange" ) ) );
    render.m_entities.push_back(
        new Sphere( Vec3( 1, 0, -1 ), 0.5, render.m_materials.at( "metal.green" ) ) );
    render.m_entities.push_back( new Sphere(
        Vec3( -1, 0, -1 ), 0.5, render.m_materials.at( "metal.lightgrey" ) ) );

    render.updateWorld();
    return render;
}

RTWK1::DepthCamera createDOPCamera( float i_fov, float i_aperture )
{
    using namespace RTWK1;

    Vec3 lookfrom( 3, 3, 2 );
    Vec3 lookat( 0, 0, -1 );
    float dist_to_focus = ( lookfrom - lookat ).length();
    float aperture = i_aperture;

    return DepthCamera(
        lookfrom, lookat, Vec3( 0, 1, 0 ), i_fov, 2.0f, aperture, dist_to_focus );
}

RTWK1::Camera createCamera()
{
    using namespace RTWK1;

    Vec3 lookfrom( -1.35f, 1.45f, 0.7f );
    Vec3 lookat( 0, 0, -1 );
    return Camera( lookfrom, lookat, Vec3( 0, 1, 0 ), 60.0f, 2.0f );
}

TEST_CASE( "test_depth_of_field" )
{
    using namespace RTWK1;

    auto render = createScene();
    std::ofstream ofs;

    ofs.open( "/tmp/test_camera_depthfield_ap2.ppm" );
    DepthCamera camera = createDOPCamera( 20, 2.0f );
    RTWK1::createImageMaterial( ofs, 200, 100, 16, camera, render, true );
    ofs.close();

    ofs.open( "/tmp/test_camera_depthfield_ap1.ppm" );
    camera = createDOPCamera( 20, 1.0f );
    RTWK1::createImageMaterial( ofs, 200, 100, 16, camera, render, true );
    ofs.close();

    ofs.open( "/tmp/test_camera_depthfield_ap05.ppm" );
    camera = createDOPCamera( 20, 0.5f );
    RTWK1::createImageMaterial( ofs, 200, 100, 16, camera, render, true );
    ofs.close();
}
