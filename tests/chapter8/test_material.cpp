//
// Created by wein on 2/09/18.
//

#include <material.hh>
#include <simpleRender.hh>

#include <limits>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <cassert>
#include <cstdlib>

void lambert() {
    using namespace RTWK;

    std::ofstream ofs;
    ofs.open("/tmp/lambert.ppm");
    assert(ofs.good());
    RTWK::Camera camera;

    RTWK::SimpleRender render;
    render.m_materials["lambert.grey"] =
        new Lambertian({0.5f, 0.5f, 0.5f});

    render.m_entities.push_back(new Sphere(
        Vec3(0, 0, -1), 0.5, render.m_materials.at("lambert.grey")));
    render.m_entities.push_back(new Sphere(
        Vec3(0, -100.5f, -1), 100, render.m_materials.at("lambert.grey")));
    render.m_entities.push_back(new Sphere(
        Vec3(1, 0, -1), 0.5, render.m_materials.at("lambert.grey")));
    render.m_entities.push_back(new Sphere(
        Vec3(-1, 0, -1), 0.5, render.m_materials.at("lambert.grey")));

    render.updateWorld();

    RTWK::createImageMaterial(
        ofs,
        200, 100, 32,
        camera, render,
        true);
}

void metal(float fuzziness, const char* filename) {
    using namespace RTWK;

    std::ofstream ofs;
    ofs.open(filename);
    assert(ofs.good());
    RTWK::Camera camera;

    SimpleRender render;
    render.m_materials["lambert.red"] =
        new Lambertian({0.8f, 0.3f, 0.3f});
    render.m_materials["lambert.orange"] =
        new Lambertian({0.8f, 0.8f, 0});
    render.m_materials["metal.green"] =
        new Metal({0.8f, 0.6f, 0.2f}, fuzziness);
    render.m_materials["metal.lightgrey"] =
        new Metal({0.8f, 0.8f, 0.8f}, fuzziness);

    render.m_entities.push_back(new Sphere(
        Vec3(0, 0, -1), 0.5, render.m_materials.at("lambert.red")));
    render.m_entities.push_back(new Sphere(
        Vec3(0, -100.5f, -1), 100, render.m_materials.at("lambert.orange")));
    render.m_entities.push_back(new Sphere(
        Vec3(1, 0, -1), 0.5, render.m_materials.at("metal.green")));
    render.m_entities.push_back(new Sphere(
        Vec3(-1, 0, -1), 0.5, render.m_materials.at("metal.lightgrey")));

    render.updateWorld();

    RTWK::createImageMaterial(
        ofs,
        200, 100, 32,
        camera, render,
        true);
}

int main() {
    std::cout << "lambert test" << std::endl;
    lambert();
    std::cout << "metal test - fuzziness: 0" << std::endl;
    metal(0, "/tmp/metal_f0.ppm");
    std::cout << "metal test - fuzziness: 0.25" << std::endl;
    metal(0.25f, "/tmp/metal_f025.ppm");

    return 0;
}
