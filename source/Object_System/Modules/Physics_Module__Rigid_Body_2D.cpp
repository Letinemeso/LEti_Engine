#include <Object_System/Modules/Physics_Module__Rigid_Body_2D.h>

using namespace LEti;


INIT_FIELDS(LEti::Physics_Module__Rigid_Body_2D__Stub, LEti::Dynamic_Physics_Module_2D_Stub)

ADD_FIELD(float, mass)

FIELDS_END



LV::Variable_Base* Physics_Module__Rigid_Body_2D__Stub::M_construct_product() const
{
    return new Physics_Module__Rigid_Body_2D;
}

void Physics_Module__Rigid_Body_2D__Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Dynamic_Physics_Module_2D_Stub::M_init_constructed_product(_product);

    Physics_Module__Rigid_Body_2D* result = (Physics_Module__Rigid_Body_2D*)_product;
    result->set_mass(mass);
}



INIT_FIELDS(LEti::Physics_Module__Rigid_Body_2D, LEti::Dynamic_Physics_Module_2D)
FIELDS_END



void Physics_Module__Rigid_Body_2D::align_to_center_of_mass(Default_Draw_Module_2D* _associated_draw_module)
{
	glm::vec3 stride = -calculate_raw_center_of_mass();

	get_physical_model()->move_raw(stride);
	get_physical_model_prev_state()->update_to_current_model_state();

    if(_associated_draw_module == nullptr)
        return;

    for(unsigned int i=0; i<_associated_draw_module->vertices().size(); i += 3)
    {
        _associated_draw_module->vertices()[i] += stride.x;
        _associated_draw_module->vertices()[i + 1] += stride.y;
        _associated_draw_module->vertices()[i + 2] += stride.z;
    }
}



void Physics_Module__Rigid_Body_2D::set_mass(float _mass)
{
	m_mass = _mass;
}

void Physics_Module__Rigid_Body_2D::set_velocity(const glm::vec3 &_v)
{
    m_velocity = _v;
}

void Physics_Module__Rigid_Body_2D::set_angular_velocity(float _av)
{
    m_angular_velocity = _av;
}


void Physics_Module__Rigid_Body_2D::apply_linear_impulse(const glm::vec3 &_imp)
{
    m_velocity += _imp;
}

void Physics_Module__Rigid_Body_2D::apply_rotation(float _av)
{
    m_angular_velocity += _av;
}



float Physics_Module__Rigid_Body_2D::mass() const
{
	return m_mass;
}

const glm::vec3& Physics_Module__Rigid_Body_2D::velocity() const
{
    return m_velocity;
}

float Physics_Module__Rigid_Body_2D::angular_velocity() const
{
    return m_angular_velocity;
}


glm::vec3 Physics_Module__Rigid_Body_2D::calculate_raw_center_of_mass() const
{
    glm::vec3 result(0.0f, 0.0f, 0.0f);

    for(unsigned int i=0; i<get_physical_model()->get_polygons_count(); ++i)
        result += get_physical_model()->get_polygons()[i].center_raw();

    return result;
}
