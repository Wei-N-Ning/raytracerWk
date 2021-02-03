//
// Created by wein on 2/09/18.
//

#include "vec3.hh"
#include "material.hh"

#include <random>

namespace RTWK
{
// Ported from Chapter 7, diffuse
// need to be refactored
RTWK::Vec3 randomInUnitSphere()
{
    RTWK::Vec3 p;
    static std::mt19937 gen{ std::random_device()() };
    static std::uniform_real_distribution< float > dist( 0.0, 1.0 );
    do
    {
        RTWK::Vec3 s = 2.0f * RTWK::Vec3( dist( gen ), dist( gen ), dist( gen ) );
        p = s - RTWK::Vec3( 1, 1, 1 );
    } while ( p.sqr_length() >= 1.0f );
    return p;
}

bool Lambertian::scatter( const RTWK::Ray &inRay,
                          const RTWK::HitRecord &record,
                          RTWK::Vec3 &attenuation,
                          RTWK::Ray &scattered ) const
{
    Vec3 target = record.p + record.normal + randomInUnitSphere();

    // recall: send a new ray from the hit point
    scattered = Ray( record.p, target - record.p );

    // recall: definition of albedo
    attenuation = m_albedo;

    // always scattered
    return true;
}

// P26
// ray reflection formula:
// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
// the key is to understand the project of v (in-ray) in the direction of n

// NOTE: here is my explanation:
// let v' be the opposite of v, that shoots off from B's original, then
// reflected = 2B - v'
//           = 2B - (-v)
//           = 2B + v
// B is in the direction of N, hence the * N in the calculation below;
// the length of B is -dot(v, N) since both are unit vectors;
// the minus sign is needed because v and N are from opposite directions
Vec3 reflect( const Vec3 &v, const Vec3 &N )
{
    return v - 2 * dot( v, N ) * N;
}

bool Metal::scatter( const RTWK::Ray &inRay,
                     const RTWK::HitRecord &record,
                     RTWK::Vec3 &attenuation,
                     RTWK::Ray &scattered ) const
{
    Vec3 reflected = reflect( inRay.direction().normalized(), record.normal );

    scattered = Ray( record.p, reflected + m_fuzziness * randomInUnitSphere() );
    attenuation = m_albedo;

    // only scattered if there is an acute angle between incoming ray
    // and the surface normal;
    return ( dot( scattered.direction(), record.normal ) > 0 );
}

// P30
// also see paper:
// 2006-greve-reflection-and-refraction
bool refract( const Vec3 &i_v, const Vec3 &i_n, float i_ni_over_nt, Vec3 &o_refracted )
{
    Vec3 uv = i_v.normalized();

    // the example in his book does not normalize the normal
    Vec3 un = i_n.normalized();

    // theta
    float dt = dot( uv, un );
    float discriminant = 1.0f - i_ni_over_nt * i_ni_over_nt * ( 1.0f - dt * dt );
    if ( discriminant > 0.0f )
    {
        o_refracted = i_ni_over_nt * ( uv - un * dt ) - un * std::sqrt( discriminant );
        return true;
    }
    return false;
}

// P32
float schlick( float cosine, float ref_idx )
{
    float r0 = ( 1 - ref_idx ) / ( 1 + ref_idx );
    r0 = r0 * r0;
    return r0 + ( 1 - r0 ) * pow( ( 1 - cosine ), 5 );
}

// P31
// dielectric material always refracts when possible
// attenuation is always 1: the glass surface absorbs nothing
// attenuation = Vec3(1.0, 1.0, 0.0) will also kill the blue channel,
// which is the type of color bug that can be hard to find

bool Dielectric::scatter( const Ray &inRay,
                          const HitRecord &record,
                          Vec3 &attenuation,
                          Ray &scattered ) const
{
    Vec3 outward_normal;
    Vec3 reflected = reflect( inRay.direction(), record.normal );
    float ni_over_nt;
    attenuation = Vec3( 1.0, 1.0, 1.0 );
    Vec3 refracted;
    float reflect_prob;
    float cosine;

    if ( dot( inRay.direction(), record.normal ) > 0 )
    {
        outward_normal = -record.normal;
        ni_over_nt = ref_idx;
        cosine = ref_idx * dot( inRay.direction(), record.normal )
                 / inRay.direction().length();
    }
    else
    {
        outward_normal = record.normal;
        ni_over_nt = 1.0f / ref_idx;
        cosine = -dot( inRay.direction(), record.normal ) / inRay.direction().length();
    }

    if ( refract( inRay.direction(), outward_normal, ni_over_nt, refracted ) )
    {
        reflect_prob = schlick( cosine, ref_idx );
    }
    else
    {
        scattered = Ray( record.p, reflected );
        reflect_prob = 1.0f;
    }

    if ( drand48() < reflect_prob )
    {
        scattered = Ray( record.p, reflected );
    }
    else
    {
        scattered = Ray( record.p, refracted );
    }

    return true;
}

}  // namespace RTWK