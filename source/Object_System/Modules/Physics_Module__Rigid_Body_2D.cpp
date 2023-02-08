#include <Object_System/Modules/Physics_Module__Rigid_Body_2D.h>

using namespace LEti;


void Physics_Module__Rigid_Body_2D::align_to_center_of_mass()
{
	glm::vec3 stride = -calculate_raw_center_of_mass();

	get_physical_model()->move_raw(stride);
	get_physical_model_prev_state()->update_to_current_model_state();
}

void Physics_Module__Rigid_Body_2D::set_mass(float _mass)
{
	m_mass = _mass;
}



float Physics_Module__Rigid_Body_2D::mass() const
{
	return m_mass;
}


glm::vec3 Physics_Module__Rigid_Body_2D::calculate_raw_center_of_mass() const
{
	glm::vec3 result(0.0f, 0.0f, 0.0f);

	for(unsigned int i=0; i<get_physical_model()->get_polygons_count(); ++i)
		result += get_physical_model()->get_polygons()[i].center_raw();

	return result;
}
