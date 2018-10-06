//
// Created by wein on 6/10/18.
//

#include <simpleRender.hh>
#include <material.hh>

#include <fstream>
#include <cassert>


void test_camera_fov() {
    using namespace RTWK;

    SimpleRender render;
    render.m_materials["lambert.red"] =
        new Lambertian({1.0f, 0.1f, 0.0f});
    render.m_materials["lambert.blue"] =
        new Lambertian({0.0f, 0.3f, 1.0f});
    render.m_entities.push_back(new Sphere(
        Vec3(-0.4f, 0, -1), 0.4f, render.m_materials.at("lambert.blue")));
    render.m_entities.push_back(new Sphere(
        Vec3(0.8f, 0, -1), 0.8f, render.m_materials.at("lambert.red")));
    render.updateWorld();

    RTWK::Camera camera;
    std::ofstream ofs;

    camera = Camera(45.0f, 2.0f);
    ofs.open("/tmp/test_camera_fov45.ppm");
    RTWK::createImageMaterial(ofs, 200, 100, 8, camera, render, true);
    ofs.close();

    camera = Camera(90.0f, 2.0f);
    ofs.open("/tmp/test_camera_fov90.ppm");
    RTWK::createImageMaterial(ofs, 200, 100, 8, camera, render, true);
    ofs.close();

    camera = Camera(120.0f, 2.0f);
    ofs.open("/tmp/test_camera_fov120.ppm");
    RTWK::createImageMaterial(ofs, 200, 100, 8, camera, render, true);
    ofs.close();
}


int main() {
    test_camera_fov();

    return 0;
}

