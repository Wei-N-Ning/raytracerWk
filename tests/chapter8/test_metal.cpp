//
// Created by wein on 2/09/18.
//

#include <ppm.hh>
#include <entities.hh>
#include <material.hh>

#include <limits>
#include <fstream>
#include <random>
#include <cassert>
#include <cstdlib>

constexpr float maxFloat = std::numeric_limits<float>::max();

RTWK::Vec3 surfaceColor(RTWK::Ray& ray) {
    using namespace RTWK;

    static std::vector<Sphere> store{
        Sphere(Vec3{0, 0, -1}, 0.5),
        Sphere(Vec3{0, -100.5f, -1}, 100)
    };
    static Lambertian material({0.8f, 0.3f, 0.3f});

    HitableList spheres;
    spheres.add(&(store[0]));
    spheres.add(&(store[1]));

    HitRecord hitRecord;
    hitRecord.material = &material;

    if (spheres.hit(ray, 0.00001f, maxFloat, hitRecord) > 0) {
        Ray scattered;
        Vec3 attenuation;
        if (hitRecord.material->scatter(
            ray, hitRecord, attenuation, scattered
            )) {
            return attenuation * surfaceColor(scattered);
        }
        else {
            return {0, 0, 0};
        }
    }

    return backgroundColor(ray);
}

int main() {
    std::ofstream ofs;
    ofs.open("/tmp/metal.ppm");
    assert(ofs.good());
    RTWK::Camera camera;
    RTWK::createImageCamAA(
        ofs,
        200, 100, 8,
        camera, surfaceColor,
        true);
    return 0;
}
