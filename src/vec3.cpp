//
// Created by wein on 26/08/18.
//

#include "vec3.hh"

namespace RTWK {

Vec3::Vec3() {
    m_elements.assign({0, 0, 0});
}

Vec3::Vec3(float e0, float e1, float e2) {
    m_elements.assign({e0, e1, e2});
}

float Vec3::x() const {
    return m_elements[0];
}

float Vec3::y() const {
    return m_elements[1];
}

float Vec3::z() const {
    return m_elements[2];
}

float Vec3::r() const {
    return m_elements[0];
}

float Vec3::g() const {
    return m_elements[1];
}

float Vec3::b() const {
    return m_elements[2];
}

const Vec3 &Vec3::operator+() const {
    return *this;
}

Vec3 Vec3::operator-() const {
    return Vec3(-m_elements[0], -m_elements[1], -m_elements[2]);
}

float Vec3::operator[](int i) const {
    return m_elements[i];
}

float &Vec3::operator[](int i) {
    return m_elements[i];
}

Vec3 &Vec3::operator+=(const Vec3 &other) {
    m_elements[0] += other.m_elements[0];
    m_elements[1] += other.m_elements[1];
    m_elements[2] += other.m_elements[2];
    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other) {
    m_elements[0] -= other.m_elements[0];
    m_elements[1] -= other.m_elements[1];
    m_elements[2] -= other.m_elements[2];
    return *this;
}

Vec3 &Vec3::operator*=(const Vec3 &other) {
    m_elements[0] *= other.m_elements[0];
    m_elements[1] *= other.m_elements[1];
    m_elements[2] *= other.m_elements[2];
    return *this;
}

Vec3 &Vec3::operator/=(const Vec3 &other) {
    m_elements[0] /= other.m_elements[0];
    m_elements[1] /= other.m_elements[1];
    m_elements[2] /= other.m_elements[2];
    return *this;
}

Vec3 &Vec3::operator*=(float v) {
    m_elements[0] *= v;
    m_elements[1] *= v;
    m_elements[2] *= v;
    return *this;
}

Vec3 &Vec3::operator/=(float v) {
    m_elements[0] /= v;
    m_elements[1] /= v;
    m_elements[2] /= v;
    return *this;
}

float Vec3::length() const {
    return std::sqrt(
        m_elements[0] * m_elements[0] +
        m_elements[1] * m_elements[1] +
        m_elements[2] * m_elements[2]);
}

float Vec3::sqr_length() const {
    return m_elements[0] * m_elements[0] +
           m_elements[1] * m_elements[1] +
           m_elements[2] * m_elements[2];
}

Vec3 Vec3::unit() {
    return {1, 1, 1};
}

std::string Vec3::str() const {
    std::stringstream ss;
    ss << m_elements[0] << " "
       << m_elements[1] << " "
       << m_elements[2];
    return ss.str();
}

}

