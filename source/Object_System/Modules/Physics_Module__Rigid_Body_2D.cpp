#include <Object_System/Modules/Physics_Module__Rigid_Body_2D.h>

using namespace LEti;


void Physics_Module__Rigid_Body_2D::align_to_center_of_mass()
{
	glm::vec3 stride = - get_physical_model()->center_of_mass_raw();
//	stride = stride;
	get_physical_model()->move_raw(stride);
	get_physical_model_prev_state()->update_to_current_model_state();
}
