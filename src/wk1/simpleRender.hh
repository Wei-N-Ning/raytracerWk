//
// Created by wein on 5/10/18.
//

#ifndef RAYTRACER_IN_A_WEEKEND_SIMPLERENDER_HH
#define RAYTRACER_IN_A_WEEKEND_SIMPLERENDER_HH

#include "ppm.hh"
#include "entities.hh"

#include <map>

namespace RTWK
{
class SimpleRender : public RTWK::IRender
{
public:
    ~SimpleRender();

    RTWK::Vec3 render( RTWK::Ray &ray ) override;

    void updateWorld()
    {
        m_world = new RTWK::HitableList( true );
        for ( auto &ptr : m_entities )
        {
            m_world->add( ptr );
        }
    }

    RTWK::HitableList *m_world = nullptr;
    std::map< std::string, RTWK::IMaterial * > m_materials;
    std::vector< RTWK::IHitable * > m_entities;
};

}  // namespace RTWK

#endif  // RAYTRACER_IN_A_WEEKEND_SIMPLERENDER_HH
