//
// Created by wein on 2/09/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_CAMERA_HH
#define RAYTRACER_IN_A_WEEKEND_CAMERA_HH

#include "ray.hh"

namespace RTWK
{
// P33
// an adjustable field of view (fov)
// this is the angle you see through the portal
// since our image is not square, the fov is
// different horizontally and vertically

// the author's preference:
// "I always use vertical fov. I also usually specify
// it in degrees and change to radians inside a ctor"

// P34
// the get an arbitrary viewpoint.... we need a way to
// specify the roll, or sideways tilt, of the camera;
// the rotation around the look-at-look-from axis
// another way to think about it is even if you keep
// look-from and look-at constant, you can still
// rotate your head around your nose
// what we need is a way to specify an up vector for
// the camera
// notice we already have a plane what the up vector
// should be in, the plane orthogonal to the view
// direction

// use the common convention of naming a "view up"
// (vup) vector
// vup, v and w are all in the same plane
// like before when our fixed camera faced -Z, our
// arbitrary view camera faces -w
// we can - but we don't have to - use world up (0, 1, 1)
// to specify vup,

class Camera
{
public:
    Camera()
        : m_origin( { 0, 0, 0 } )
        , m_lowerLeftCorner( { -2.0f, -1.0f, -1.0f } )
        , m_horizontal( { 4.0f, 0, 0 } )
        , m_vertical( { 0, 2.0f, 0 } )
    {
    }

    Camera( float i_vfov, float i_aspect );

    Camera( Vec3 i_look_from, Vec3 i_look_at, Vec3 i_vup, float i_vfov, float i_aspect );

    virtual Ray getRay( float u, float v )
    {
        return Ray( m_origin,
                    m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin );
    }

protected:
    Vec3 m_origin;
    Vec3 m_lowerLeftCorner;
    Vec3 m_horizontal;
    Vec3 m_vertical;
};

// P38
// (on depth of field)
// we also don't need to simulate any of the inside
// of the camera.... instead I usually start rays
// from the surface of the lens, and send them toward
// a virtual film plane, by finding the projection
// of the film on the plane that is in focus ( at
// the distance focus_dist )
// for that we just need to have the ray origins be
// on a disk around look-from rather than from a
// point

class DepthCamera : public Camera
{
public:
    DepthCamera( Vec3 i_look_from,
                 Vec3 i_look_at,
                 Vec3 i_vup,
                 float i_vfov,
                 float i_aspect,
                 float i_aperture,
                 float i_focus_dist );

    Ray getRay( float s, float t ) override;

private:
    Vec3 m_u;
    Vec3 m_v;
    Vec3 m_w;

    float m_lensRadius;
};

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_CAMERA_HH
