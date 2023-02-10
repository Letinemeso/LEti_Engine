#include <Object_System/Rigid_Body_2D.h>

using namespace LEti;


INIT_FIELDS(LEti::Rigid_Body_2D, LEti::Object_2D);
FIELDS_END;



void Rigid_Body_2D::init(const LV::Variable_Base &_stub)
{
	Object_2D::init(_stub);

	glm::vec3 stride = -((Physics_Module__Rigid_Body_2D*)physics_module())->calculate_raw_center_of_mass();

	((Physics_Module__Rigid_Body_2D*)physics_module())->align_to_center_of_mass();

	for(unsigned int i=0; i<draw_module()->vertices().size(); i += 3)
	{
		draw_module()->vertices()[i] += stride.x;
		draw_module()->vertices()[i + 1] += stride.y;
		draw_module()->vertices()[i + 2] += stride.z;
	}
}



void Rigid_Body_2D::create_physics_module()
{
	delete[] m_physics_module;
	m_physics_module = new Physics_Module__Rigid_Body_2D;
}



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
	((Physics_Module__Rigid_Body_2D*)physics_module())->set_mass(_mass);
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
	return ((Physics_Module__Rigid_Body_2D*)physics_module())->mass();
}
