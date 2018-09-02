//
// Created by wein on 2/09/18.
//

#include <ppm.hh>
#include <entities.hh>
#include <material.hh>

#include <limits>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <cassert>
#include <cstdlib>

constexpr float maxFloat = std::numeric_limits<float>::max();

class SimpleRender : public RTWK::IRender {
public:
    ~SimpleRender();
    RTWK::Vec3 operator() (RTWK::Ray& ray) override;
    void updateWorld() {
        m_world = new RTWK::HitableList(true);
        for (auto& ptr : m_entities) {
            m_world->add(ptr);
        }
    }

    RTWK::HitableList* m_world = nullptr;
    std::map<std::string, RTWK::IMaterial *> m_materials;
    std::vector<RTWK::IHitable *> m_entities;
};

RTWK::Vec3 SimpleRender::operator()(RTWK::Ray &ray) {
    using namespace RTWK;

    HitRecord hitRecord;
    if (m_world->hit(ray, 0.00001f, maxFloat, hitRecord) > 0) {
        Ray scattered;
        Vec3 attenuation;
        if (hitRecord.material->scatter(
            ray, hitRecord, attenuation, scattered
        )) {
            return attenuation * (*this)(scattered);
        }
        else {
            return {0, 0, 0};
        }
    }
    return backgroundColor(ray);
}

SimpleRender::~SimpleRender() {
    for (auto& pair : m_materials) {
        delete pair.second;
    }
    for (auto& ptr : m_entities) {
        delete ptr;
    }
    delete m_world;
}

void lambert() {
    std::ofstream ofs;
    ofs.open("/tmp/lambert.ppm");
    assert(ofs.good());
    RTWK::Camera camera;

    SimpleRender render;
    using namespace RTWK;
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
        400, 200, 16,
        camera, render,
        true);
}

void metal() {
    std::ofstream ofs;
    ofs.open("/tmp/metal.ppm");
    assert(ofs.good());
    RTWK::Camera camera;

    SimpleRender render;
    using namespace RTWK;
    render.m_materials["lambert.red"] =
        new Lambertian({0.8f, 0.3f, 0.3f});
    render.m_materials["lambert.orange"] =
        new Lambertian({0.8f, 0.8f, 0});
    render.m_materials["metal.green"] =
        new Metal({0.8f, 0.6f, 0.2f});
    render.m_materials["metal.lightgrey"] =
        new Metal({0.8f, 0.8f, 0.8f});

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
        400, 200, 16,
        camera, render,
        true);
}

int main() {
    std::cout << "lambert test" << std::endl;
    lambert();
    std::cout << "metal test" << std::endl;
    metal();
    return 0;
}
