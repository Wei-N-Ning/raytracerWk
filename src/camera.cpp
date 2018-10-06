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
    m_lowerLeftCorner = m_origin - half_width * u - half_height * v - w;
    m_horizontal = 2 * half_width * u;
    m_vertical = 2 * half_height * v;
}

}