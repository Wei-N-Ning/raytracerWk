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

}