//
// Created by wein on 2/09/18.
//

#include "camera.hh"

#include <cmath>
#include <random>

namespace RTWK1
{
Camera::Camera( float vFov, float aspect )
{
    float theta = vFov * M_PI / 180.0f;
    float half_height = std::tan( theta / 2.0f );
    float half_width = aspect * half_height;

    // compare this to the default value: m_lowerLeftCorner( { -2.0f, -1.0f, -1.0f } )
    // the larger the theta (fov) is, the larger half_width and half_height are;
    // when vFov is close to 180 degree, theta will be close to PI, then tangent will
    // be close to 1.0, i.e. tan(180 / 2.0f)
    // in a way, this ctor parameterizes the view plan using the fov
    m_lowerLeftCorner = Vec3( -half_width, -half_height, -1.0f );
    m_horizontal = Vec3( 2 * half_width, 0.0f, 0.0f );
    m_vertical = Vec3( 0.0f, 2 * half_height, 0.0f );
    m_origin = Vec3( 0, 0, 0 );
}

Camera::Camera( const Vec3& lookFrom,
                const Vec3& lookAt,
                const Vec3& vup,
                float vFov,
                float aspect )
{
    Vec3 u, v, w;

    // the function of fov and aspect ratio stays the same
    float theta = vFov * M_PI / 180.0f;
    float half_height = std::tan( theta / 2.0f );
    float half_width = aspect * half_height;

    // lookFrom and lookAt enables the program to position and orient the camera
    m_origin = lookFrom;
    w = ( lookFrom - lookAt ).normalized();

    // recall cross product right hand rule
    // index (a)
    // mid (b)
    // thumb (a X b)
    u = cross( vup, w ).normalized();
    v = cross( w, u );

    // adjust the viewing plane to the non-zero origin
    m_lowerLeftCorner = m_origin - half_width * u - half_height * v - w;
    m_horizontal = 2 * half_width * u;
    m_vertical = 2 * half_height * v;
}

Vec3 randomInUnitDisk()
{
    static std::mt19937 eng{ std::random_device()() };
    static std::uniform_real_distribution< double > dist{ 0.0, 1.0 };
    Vec3 p;
    do
    {
        p = 2.0 * Vec3( dist( eng ), dist( eng ), 0 ) - Vec3( 1, 1, 0 );
    } while ( dot( p, p ) >= 1.0 );
    return p;
}

DepthCamera::DepthCamera( const Vec3& lookFrom,
                          const Vec3& lookAt,
                          const Vec3& vup,
                          float vFov,
                          float aspect,
                          float aperture,
                          float focusDist )
    : Camera()
{
    m_lensRadius = aperture / 2.0f;
    float theta = vFov * M_PI / 180.0f;
    float half_height = tan( theta / 2.0f );
    float half_width = aspect * half_height;
    m_origin = lookFrom;
    m_w = ( lookFrom - lookAt ).normalized();
    m_u = cross( vup, m_w ).normalized();
    m_v = cross( m_w, m_u );
    m_lowerLeftCorner = m_origin - half_width * focusDist * m_u
                        - half_height * focusDist * m_v - focusDist * m_w;
    m_horizontal = 2 * half_width * focusDist * m_u;
    m_vertical = 2 * half_height * focusDist * m_v;
}

Ray DepthCamera::getRay( float s, float t )
{
    Vec3 random_ray = m_lensRadius * randomInUnitDisk();
    Vec3 offset = m_u * random_ray.x() + m_v * random_ray.y();
    return Ray(
        m_origin + offset,
        m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset );
}

}  // namespace RTWK