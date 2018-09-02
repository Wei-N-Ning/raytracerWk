//
// Created by wein on 27/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_ENTITIES_HH
#define RAYTRACER_IN_A_WEEKEND_ENTITIES_HH

#include "ray.hh"
#include "vec3.hh"

#include <vector>

namespace RTWK {

// P24
// Hitable and Material needs to know each other so there is some
// circularity of the references.
// In C++ you just need to alert the compiler that the pointer to a
// class "Material".
class IMaterial;

struct HitRecord {
    float t = 0;
    Vec3 p;
    Vec3 normal;
    IMaterial* material = nullptr;
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

// Like the example in the book, HitablaList does not own the Hitable
// objects. It does not offer any means to free these resources.
class HitableList : public Hitable {
public:
    HitableList() noexcept
        : m_hitables() {
    }

    std::size_t size() const {
        return m_hitables.size();
    }

    void add(Hitable* hitable) {
        m_hitables.push_back(hitable);
    }

    bool hit(
        const Ray& ray,
        float t_min,
        float t_max,
        HitRecord& record) override;

private:
    std::vector<Hitable *> m_hitables;
};

}

#endif //RAYTRACER_IN_A_WEEKEND_ENTITIES_HH
