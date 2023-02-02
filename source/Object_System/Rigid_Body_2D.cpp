#include <Object_System/Rigid_Body_2D.h>

using namespace LEti;



void Rigid_Body_2D::update(float _ratio)
{
	move(m_velocity * LEti::Event_Controller::get_dt() * _ratio);

	rotate(m_angular_velocity * LEti::Event_Controller::get_dt() * _ratio);

	LEti::Object_2D::update();
}



void Rigid_Body_2D::set_velocity(const glm::vec3 &_v)
{
	m_velocity = _v;
}

void Rigid_Body_2D::set_angular_velocity(float _av)
{
	m_angular_velocity = _av;
}

void Rigid_Body_2D::set_mass(float _mass)
{
	m_mass = _mass;
}


void Rigid_Body_2D::apply_linear_impulse(const glm::vec3 &_imp)
{
	m_velocity += _imp;
}

void Rigid_Body_2D::apply_rotation(float _av)
{
	m_angular_velocity += _av;
}



const glm::vec3& Rigid_Body_2D::velocity() const
{
	return m_velocity;
}

float Rigid_Body_2D::angular_velocity() const
{
	return m_angular_velocity;
}

float Rigid_Body_2D::mass() const
{
	return m_mass;
}
