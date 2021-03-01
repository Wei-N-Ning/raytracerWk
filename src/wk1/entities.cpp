//
// Created by wein on 27/08/18.
//

#include <utility>

#include "entities.hh"

namespace RTWK
{
Sphere::Sphere( RTWK::Vec3 center, float radius )
    : m_center(std::move( center )), m_radius( radius )
{
}

Sphere::Sphere( RTWK::Vec3 center, float radius, RTWK::IMaterial *material )
    : m_center(std::move( center )), m_radius( radius ), m_material( material )
{
}

// P/10 the hit-test
// we have the sphere equation:
// dot((p - C), (p - C) = R^2
// (recall the meaning of p - C is a vector from C to p)
// we plug in the ray equation: p(t) = A + tB
// the problem becomes:
// find any t in range [t_min, t_max] that results in a p on the sphere
// i.e.
// dot((A + tB - C), (A + tB - C)) = R^2
// A: ray's origin
// B: ray's direction (should be normalized)
// C: center of sphere
bool Sphere::hit( const RTWK::Ray &ray, float t_min, float t_max, HitRecord &record )
{
    Vec3 centerToOrigin = ray.origin() - m_center;

    float a = dot( ray.direction(), ray.direction() );

    float b = 2.0f * dot( centerToOrigin, ray.direction() );

    float c = dot( centerToOrigin, centerToOrigin ) - m_radius * m_radius;

    float discriminant = b * b - 4 * a * c;

    // no hit, discard this ray
    if ( discriminant < 0 )
    {
        return false;
    }

    // find the first root to get the near hit point
    float t = ( -b - std::sqrt( discriminant ) ) / ( 2.0f * a );

    // the near hit point t is within (t_min, t_max)
    if ( t < t_max && t > t_min )
    {
        // create a hit record from this t
        record.t = t;
        record.p = ray.pointAtParameter( record.t );
        record.normal = ( ( record.p - m_center ) / m_radius ).normalized();
        record.material = m_material;
        return true;
    }

    // the near hit point t is outside (t_min, t_max);
    // find the second root to try to get the far hit point
    t = ( -b + std::sqrt( discriminant ) ) / ( 2.0f * a );
    if ( t < t_max && t > t_min )
    {
        // create a hit record from this t
        record.t = t;
        record.p = ray.pointAtParameter( record.t );
        record.normal = ( ( record.p - m_center ) / m_radius ).normalized();
        record.material = m_material;
        return true;
    }

    // the far hit point is outside the range; discard
    return false;
}

bool HitableList::hit( const RTWK::Ray &ray,
                       float t_min,
                       float t_max,
                       RTWK::HitRecord &record )
{
    RTWK::HitRecord tempRecord;
    bool hitAnything = false;
    float closestSoFar = t_max;

    for ( IHitable *hitable : m_hitables )
    {
        if ( hitable->hit( ray, t_min, closestSoFar, tempRecord ) )
        {
            // hit something and the hit point is recorded
            hitAnything = true;
            // shrink the search range to (t_min, this_hit_point) so that we only look
            // for any other hitable within this range, not looking any further
            closestSoFar = tempRecord.t;
            if ( m_supportMaterial )
            {
                // thread the hit record with material to the caller
                record = tempRecord;
            }
            else
            {
                // discard the hit record's material ('cos it won't have any)
                record.t = tempRecord.t;
                record.normal = tempRecord.normal;
                record.p = tempRecord.p;
            }
        }
    }

    return hitAnything;
}

}  // namespace RTWK