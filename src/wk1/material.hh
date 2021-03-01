//
// Created by wein on 2/09/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_MATERIAL_HH
#define RAYTRACER_IN_A_WEEKEND_MATERIAL_HH

// Chapter 8
// P24
// "if we want different objects to have different materials, we have
// a design decision. we could have a universal material with lots of
// parameters and different material types just zero out some of those
// parameters. This is not a bad approach. Or we could have an abstract
// material class that encapsulates behavior. I am a fan of the latter
// approach."

#include "ray.hh"
#include "entities.hh"

namespace RTWK1
{
// P25
// "material will tell us how rays interact with the surface.
// when ray hits a surface (a particular sphere for example), the
// material pointer in the HitRecord will be set to point at the
// material pointer the sphere was given when it was set up in main()
//
// when the color() routine (ray function) gets the HitRecord it can call member
// functions of the material pointer to find out what ray, if any, is
// scattered."

class IMaterial
{
public:
    virtual bool scatter( const Ray& inRay,
                          const HitRecord& record,
                          Vec3& attenuation,
                          Ray& scattered ) const = 0;

    virtual ~IMaterial() = default;
};

// P25
// For the Lambertian (diffuse) case it can either
// > scatter always and attenuate by its reflectance R
// > or it can scatter with no attenuation but absorb the fraction
//   1 - R of the rays
// > or it could be a mixture of those strategies

// read:
// https://www.cs.cmu.edu/afs/cs/academic/class/15462-f09/www/lec/lec8.pdf
// http://www.curry.eas.gatech.edu/Courses/6140/ency/Chapter9/Ency_Atmos/Reflectance_Albedo_Surface.pdf
// (downloaded on gunship)

// albedo:
// is the measure of the diffuse reflection of solar radiation out of the total solar
// radiation received by an astronomical body (e.g. a planet like Earth).
// https://en.wikipedia.org/wiki/Albedo

class Lambertian : public IMaterial
{
public:
    explicit Lambertian( Vec3 albedo ) : m_albedo( std::move( albedo ) )
    {
    }

    bool scatter( const Ray& inRay,
                  const HitRecord& record,
                  Vec3& attenuation,
                  Ray& scattered ) const override;

private:
    Vec3 m_albedo;
};

// P25
// For smooth metals the ray won't be randomly scattered.
// the key math is: how does a ray get reflected from a metal mirror?
class Metal : public IMaterial
{
public:
    explicit Metal( Vec3 albedo ) : m_albedo( std::move( albedo ) )
    {
    }

    Metal( Vec3 albedo, float fuzziness )
        : m_albedo( std::move( albedo ) ), m_fuzziness( fuzziness )
    {
    }

    bool scatter( const Ray& inRay,
                  const HitRecord& record,
                  Vec3& attenuation,
                  Ray& scattered ) const override;

private:
    Vec3 m_albedo;

    // P28
    // The bigger the sphere (m_fuzziness), the fuzzier the
    // reflection will be
    float m_fuzziness = 0;
};

// P31
// glass
class Dielectric : public IMaterial
{
public:
    explicit Dielectric( float i_ri ) : ref_idx( i_ri )
    {
        ;
    }

    bool scatter( const Ray& inRay,
                  const HitRecord& record,
                  Vec3& attenuation,
                  Ray& scattered ) const override;

private:
    float ref_idx;
};

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_MATERIAL_HH
