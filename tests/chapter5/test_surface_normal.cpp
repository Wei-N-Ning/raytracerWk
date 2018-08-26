//
// Created by wein on 26/08/18.
//

#include <ppm.hh>
#include <vec3.hh>
#include <ray.hh>

#include <fstream>
#include <cassert>

float hitPoint(
    const RTWK::Vec3& center, float radius, const RTWK::Ray& ray) {
    using namespace RTWK;

    Vec3 centerToOrigin = ray.origin() - center;
    float a = dot(ray.direction(), ray.direction());
    float b = 2.0f * dot(centerToOrigin, ray.direction());
    float c = dot(centerToOrigin, centerToOrigin) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0f;
    }
    // compute the smaller root (closed hit point)
    return (-b - std::sqrt(discriminant)) / (2.0f * a);
}

constexpr float RADIUS = 0.5f;
RTWK::Vec3 surfaceColor(RTWK::Ray& ray) {
    using namespace RTWK;
    Vec3 center{0, 0, -1};
    float t = hitPoint(center, RADIUS, ray);
    if (t > 0) {
        Vec3 N = (ray.pointAtParameter(t) - center).normalized();
        return 0.5f * Vec3(N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f);
    }
    return backgroundColor(ray);
}

int main() {
    std::ofstream ofs;
    ofs.open("/tmp/normal.ppm");
    assert(ofs.good());
    RTWK::createImage(ofs, 200, 100, surfaceColor);
    return 0;
}