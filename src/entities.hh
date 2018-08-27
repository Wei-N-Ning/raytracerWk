//
// Created by wein on 27/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_ENTITIES_HH
#define RAYTRACER_IN_A_WEEKEND_ENTITIES_HH

#include "ray.hh"
#include "vec3.hh"

namespace RTWK {

struct HitRecord {
    float t = 0;
    Vec3 p;
    Vec3 normal;
};

class Hitable {
public:
    virtual bool hit(
        const Ray& ray,
        float t_min,
        float t_max,
        HitRecord& record) = 0;
};

class Sphere : public Hitable {
public:
    Sphere() = default;
    Sphere(const Vec3& center, float radius);

    bool hit(
        const Ray& ray,
        float t_min,
        float t_max,
        HitRecord& record) override;

private:
    Vec3 m_center;
    float m_radius = 0;
};

}

#endif //RAYTRACER_IN_A_WEEKEND_ENTITIES_HH
