//
// Created by wein on 26/08/18.
//

#include <utility>

#include "ray.hh"

namespace RTWK1
{
Ray::Ray( RTWK1::Vec3  A, RTWK1::Vec3  B ) : m_A(std::move( A )), m_B(std::move( B ))
{
}

const Vec3& Ray::origin() const
{
    return m_A;
}

const Vec3& Ray::direction() const
{
    return m_B;
}

Vec3 Ray::pointAtParameter( float t ) const
{
    return m_A + t * m_B;
}

}  // namespace RTWK