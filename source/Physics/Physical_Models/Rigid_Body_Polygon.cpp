#include <Physics/Physical_Models/Rigid_Body_Polygon.h>

using namespace LEti;


Rigid_Body_Polygon::Rigid_Body_Polygon()
    : Polygon()
{

}

Rigid_Body_Polygon::Rigid_Body_Polygon(const Rigid_Body_Polygon &_other)
    : Polygon(_other)
{
    m_mass = _other.m_mass;
}

Rigid_Body_Polygon::~Rigid_Body_Polygon()
{

}


void Rigid_Body_Polygon::setup(const Polygon &_other)
{
    Polygon::setup(_other);

    const Rigid_Body_Polygon& other = (const Rigid_Body_Polygon&)_other;

    m_mass = other.m_mass;
}
