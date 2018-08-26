//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_VEC3_HH
#define RAYTRACER_IN_A_WEEKEND_VEC3_HH

#include <istream>
#include <ostream>
#include <vector>
#include <cmath>

namespace RTWK {

class Vec3 {
public:
    Vec3();

    Vec3(float e0, float e1, float e2);

    float x() const;

    float y() const;

    float z() const;

    float r() const;

    float g() const;

    float b() const;

    const Vec3 &operator+() const;

    Vec3 operator-() const;

    float operator[](int i) const;

    float &operator[](int i);

    Vec3 &operator+=(const Vec3 &other);

    Vec3 &operator-=(const Vec3 &other);

    Vec3 &operator*=(const Vec3 &other);

    Vec3 &operator*=(float v);

    Vec3 &operator/=(const Vec3 &other);

    Vec3 &operator/=(float v);

    float length() const;

    float sqr_length() const;

    static Vec3 unit();

    friend std::istream& operator>> (std::istream& is, Vec3& v);
    friend std::ostream& operator<< (std::ostream& os, const Vec3& v);

private:
    std::vector<float> m_elements;
};

std::istream& operator>> (std::istream& is, Vec3& v);
std::ostream& operator<< (std::ostream& os, const Vec3& v);

}

#endif //RAYTRACER_IN_A_WEEKEND_VEC3_HH
