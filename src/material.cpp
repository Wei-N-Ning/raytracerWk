//
// Created by wein on 2/09/18.
//

#include "vec3.hh"
#include "material.hh"

#include <random>

namespace RTWK {

// Ported from Chapter 7, diffuse
// need to be refactored
RTWK::Vec3 randomInUnitSphere() {
    RTWK::Vec3 p;
    static std::default_random_engine gen;
    static std::uniform_real_distribution<float> dist(0, 1);
    do {
        RTWK::Vec3 s = 2.0f * RTWK::Vec3(dist(gen), dist(gen), dist(gen));
        p = s - RTWK::Vec3(1, 1, 1);
    } while (p.sqr_length() >= 1.0f);
    return p;
}

bool Lambertian::scatter(
    const RTWK::Ray &inRay,
    const RTWK::HitRecord &record,
    RTWK::Vec3 &attenuation,
    RTWK::Ray &scattered) const {

    Vec3 target =
        record.p + record.normal + randomInUnitSphere();

    // recall: send a new ray from the hit point
    scattered = Ray(record.p, target - record.p);

    // recall: definition of albedo
    attenuation = m_albedo;

    // always scattered
    return true;
}

// P26
// NOTE: here is my explanation:
// let v' be the opposite of v, that shoots off from B's original, then
// reflected = 2B - v'
//           = 2B - (-v)
//           = 2B + v
// B is in the direction of N, hence the * N in the calculation below;
// the length of B is -dot(v, N) since both are unit vectors;
// the minus sign is needed because v and N are from opposite directions
Vec3 reflect(const Vec3& v, const Vec3& N) {
    return v - 2 * dot(v, N) * N;
}

bool Metal::scatter(
    const RTWK::Ray &inRay,
    const RTWK::HitRecord &record,
    RTWK::Vec3 &attenuation,
    RTWK::Ray &scattered) const {

    Vec3 reflected = reflect(inRay.direction().normalized(),
        record.normal);
    scattered = Ray(record.p, reflected);
    attenuation = m_albedo;

    // only scattered if there is a right angle between incoming ray
    // and the surface normal;
    return (dot(scattered.direction(), record.normal) > 0);
}

}