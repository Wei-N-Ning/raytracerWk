//
// Created by wein on 2/09/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_CAMERA_HH
#define RAYTRACER_IN_A_WEEKEND_CAMERA_HH

#include "ray.hh"

namespace RTWK {

class Camera {
public:
    Camera()
        : m_origin({0, 0, 0}),
          m_lowerLeftCorner({-2.0f, -1.0f, -1.0f}),
          m_horizontal({4.0f, 0, 0}),
          m_vertical({0, 2.0f, 0}) {
    }

    Ray getRay(float u, float v) {
        return Ray(m_origin,
            m_lowerLeftCorner +
            u * m_horizontal + v * m_vertical - m_origin
        );
    }

private:
    Vec3 m_origin;
    Vec3 m_lowerLeftCorner;
    Vec3 m_horizontal;
    Vec3 m_vertical;
};

}

#endif //RAYTRACER_IN_A_WEEKEND_CAMERA_HH
