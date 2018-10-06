//
// Created by wein on 2/09/18.
//

#include "camera.hh"

namespace RTWK {

Camera::Camera(float i_vfov, float i_aspect) {
    float theta = i_vfov * M_PI / 180.0f;
    float half_height = tan(theta / 2.0f);
    float half_width = i_aspect * half_height;

    m_lowerLeftCorner = Vec3(-half_width, -half_height, -1.0f);
    m_horizontal = Vec3(2 * half_width, 0.0f, 0.0f);
    m_vertical = Vec3(0.0f, 2 * half_height, 0.0f);
    m_origin = Vec3(0, 0, 0);
}

Camera::Camera(Vec3 i_look_from, Vec3 i_look_at, Vec3 i_vup,
               float i_vfov, float i_aspect) {
    Vec3 u, v, w;
    float theta = i_vfov * M_PI / 180.0f;
    float half_height = tan(theta / 2.0f);
    float half_width = i_aspect * half_height;
    m_origin = i_look_from;
    w = (i_look_from - i_look_at).normalized();

    // recall cross product right hand rule
    // index (a)
    // mid (b)
    // thumb (a X b)
    u = cross(i_vup, w).normalized();
    v = cross(w, u);
    m_lowerLeftCorner = m_origin -
        half_width * u -
        half_height * v - w;
    m_horizontal = 2 * half_width * u;
    m_vertical = 2 * half_height * v;
}

Vec3 randomInUnitDisk() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(drand48(), drand48(), 0) - Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

DepthCamera::DepthCamera(Vec3 i_look_from, Vec3 i_look_at, Vec3 i_vup,
                         float i_vfov, float i_aspect,
                         float i_aperture, float i_focus_dist)
    : Camera() {
    m_lensRadius = i_aperture / 2.0f;
    float theta = i_vfov * M_PI / 180.0f;
    float half_height = tan(theta / 2.0f);
    float half_width = i_aspect * half_height;
    m_origin = i_look_from;
    m_w = (i_look_from - i_look_at).normalized();
    m_u = cross(i_vup, m_w).normalized();
    m_v = cross(m_w, m_u);
    m_lowerLeftCorner = m_origin -
        half_width * i_focus_dist * m_u -
        half_height * i_focus_dist * m_v -
        i_focus_dist * m_w;
    m_horizontal = 2 * half_width * i_focus_dist * m_u;
    m_vertical = 2 * half_height * i_focus_dist * m_v;

}

Ray DepthCamera::getRay(float s, float t) {
    Vec3 random_ray = m_lensRadius * randomInUnitDisk();
    Vec3 offset = m_u * random_ray.x() + m_v * random_ray.y();
    return Ray(
        m_origin + offset,

        m_lowerLeftCorner +
        s * m_horizontal + t * m_vertical -
        m_origin - offset
        );
}

}