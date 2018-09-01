//
// Created by wein on 27/08/18.
//

#include "entities.hh"

namespace RTWK {

Sphere::Sphere(const RTWK::Vec3 &center, float radius)
    : m_center(center), m_radius(radius) {}

bool Sphere::hit(
    const RTWK::Ray &ray,
    float t_min,
    float t_max,
    HitRecord &record) {
    Vec3 centerToOrigin = ray.origin() - m_center;

    float a = dot(ray.direction(), ray.direction());

    float b = 2.0f * dot(centerToOrigin, ray.direction());

    float c = dot(centerToOrigin, centerToOrigin) - m_radius * m_radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    }

    float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
    if (t < t_max && t > t_min) {
        record.t = t;
        record.p = ray.pointAtParameter(record.t);
        record.normal = ((record.p - m_center) / m_radius).normalized();
        return true;
    }

    t = (-b + std::sqrt(discriminant)) / (2.0f * a);
    if (t < t_max && t > t_min) {
        record.t = t;
        record.p = ray.pointAtParameter(record.t);
        record.normal = ((record.p - m_center) / m_radius).normalized();
        return true;
    }

    return false;
}

bool HitableList::hit(const RTWK::Ray &ray,
                      float t_min, float t_max,
                      RTWK::HitRecord &record) {
    RTWK::HitRecord tempRecord;
    bool hitAnything = false;
    float closestSoFar = t_max;

    for (Hitable* hitable : m_hitables) {
        if (hitable->hit(ray, t_min, closestSoFar, tempRecord)) {
            hitAnything = true;
            closestSoFar = tempRecord.t;
            record = tempRecord;
        }
    }

    return hitAnything;
}

}