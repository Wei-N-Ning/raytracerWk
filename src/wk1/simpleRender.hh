//
// Created by wein on 5/10/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_SIMPLERENDER_HH
#define RAYTRACER_IN_A_WEEKEND_SIMPLERENDER_HH

#include "ppm.hh"
#include "entities.hh"

#include <map>

namespace RTWK1
{
class SimpleRender : public RTWK1::IRender
{
public:
    ~SimpleRender();

    RTWK1::Vec3 render( RTWK1::Ray &ray ) override;

    void updateWorld()
    {
        m_world = new RTWK1::HitableList( true );
        for ( auto &ptr : m_entities )
        {
            m_world->add( ptr );
        }
    }

    RTWK1::HitableList *m_world = nullptr;
    std::map< std::string, RTWK1::IMaterial * > m_materials;
    std::vector< RTWK1::IHitable * > m_entities;
};

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_SIMPLERENDER_HH
