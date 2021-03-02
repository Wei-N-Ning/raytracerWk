//
// Created by wein on 5/10/18.
//

/*
 * P29
 *
 * Clear materials such as water, glass and diamonds and dielectrics.
 *
 * When a light ray hits them, it splits into a reflected and a refracted
 * (transmitted) ray.
 *
 * #Recall wt's gsz's diffTransmitRatio
 *
 * P30
 * The refraction is described by Snell's law
 *
 * n sin(theta) = n' sin(theta')
 * where n and n' are the refractive indices, typically air = 1, glass =
 * 1.3 - 1.7, diamond = 2.4
 *
 *
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <simpleRender.hh>
#include <material.hh>

#include <fstream>
#include <iostream>

// NOTE: there is still a bug in the implementation that will cause stack overflow.
//       dielectric material causes the number of ray bounces to explode dramatically!
//       therefore it MUST SET A UPPER BOUND on the bounces.
//       exam2 fixes this problem using this method

TEST_CASE( "dielectric and glass material" )
{
    using namespace RTWK1;

    std::string filename{ "/tmp/dielectric.ppm" };

    std::ofstream ofs;
    ofs.open( filename );
    CHECK( ofs.good() );
    RTWK1::Camera camera;

    SimpleRender render;

    render.m_materials[ "lambert.blue" ] = new Lambertian( { 0.1f, 0.2f, 0.5f } );

    render.m_materials[ "lambert.green" ] = new Lambertian( { 0.8f, 0.8f, 0.0f } );

    render.m_materials[ "metal.green" ] = new Metal( { 0.8f, 0.6f, 0.2f } );

    render.m_materials[ "dielectric" ] = new Dielectric( 1.5f );

    render.m_entities.push_back(
        new Sphere( Vec3( 0, 0, -1 ), 0.5, render.m_materials.at( "lambert.blue" ) ) );

    render.m_entities.push_back( new Sphere(
        Vec3( 0, -100.5f, -1 ), 100, render.m_materials.at( "lambert.green" ) ) );

    render.m_entities.push_back(
        new Sphere( Vec3( 1, 0, -1 ), 0.5, render.m_materials.at( "metal.green" ) ) );

    render.m_entities.push_back(
        new Sphere( Vec3( -1, 0, -1 ), 0.5f, render.m_materials.at( "dielectric" ) ) );

    render.m_entities.push_back(
        new Sphere( Vec3( -1, 0, -1 ), -0.45f, render.m_materials.at( "dielectric" ) ) );

    render.updateWorld();

    RTWK1::createImageMaterial( ofs, 20, 10, 12, camera, render, true );
}
