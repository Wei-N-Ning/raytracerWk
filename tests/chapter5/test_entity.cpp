//
// Created by wein on 27/08/18.
//

#include <entities.hh>
#include <ppm.hh>

#include <fstream>
#include <limits>
#include <cassert>

RTWK::Vec3 surfaceColor(RTWK::Ray& ray) {
    using namespace RTWK;

    RTWK::Sphere sphere({0, 0, -1}, 0.5);
    RTWK::HitRecord hitRecord;
    if (sphere.hit(
        ray,
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(),
        hitRecord) > 0) {
        Vec3 N = hitRecord.normal;
        return 0.5f * Vec3(N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f);
    }
    return backgroundColor(ray);
}

int main() {
    std::ofstream ofs;
    ofs.open("/tmp/entity.ppm");
    assert(ofs.good());
    RTWK::createImage(ofs, 200, 100, surfaceColor);
    return 0;
}
