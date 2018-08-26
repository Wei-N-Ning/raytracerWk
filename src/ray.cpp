//
// Created by wein on 26/08/18.
//

#include "ray.hh"

namespace RTWK {

Ray::Ray(const RTWK::Vec3 &A, const RTWK::Vec3 &B)
    : m_A(A), m_B(B) {}

const Vec3& Ray::origin() const {
    return m_A;
}

const Vec3& Ray::direction() const {
    return m_B;
}

Vec3 Ray::pointAtParameter(float t) const {
    return m_A + t * m_B;
}

}