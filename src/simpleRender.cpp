//
// Created by wein on 5/10/18.
//

#include "simpleRender.hh"
#include "material.hh"

#include <limits>

namespace RTWK
{
constexpr float maxFloat = std::numeric_limits< float >::max();

RTWK::Vec3 SimpleRender::render( RTWK::Ray& ray )
{
    using namespace RTWK;

    HitRecord hitRecord;
    if ( m_world->hit( ray, 0.00001f, maxFloat, hitRecord ) > 0 )
    {
        Ray scattered;
        Vec3 attenuation;
        if ( hitRecord.material->scatter( ray, hitRecord, attenuation, scattered ) )
        {
            return attenuation * render( scattered );
        }
        else
        {
            return { 0, 0, 0 };
        }
    }
    return generateBackgroundColor( ray );
}

SimpleRender::~SimpleRender()
{
    for ( auto& pair : m_materials )
    {
        delete pair.second;
    }
    for ( auto& ptr : m_entities )
    {
        delete ptr;
    }
    delete m_world;
}

}  // namespace RTWK