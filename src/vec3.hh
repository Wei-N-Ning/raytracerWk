//
// Created by wein on 26/08/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_VEC3_HH
#define RAYTRACER_IN_A_WEEKEND_VEC3_HH

#include <sstream>
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

    std::string str() const;

private:
    std::vector<float> m_elements;
};

}

#endif //RAYTRACER_IN_A_WEEKEND_VEC3_HH
