#include <Object_System/Modules/Physics_Module__Rigid_Body_2D.h>

using namespace LEti;


INIT_FIELDS(LEti::Physics_Module__Rigid_Body_2D__Stub, LEti::Dynamic_Physics_Module_2D_Stub)

ADD_FIELD(float*, masses)
ADD_FIELD(float, mass_multiplier)

FIELDS_END



LV::Variable_Base* Physics_Module__Rigid_Body_2D__Stub::M_construct_product() const
{
    return new Physics_Module__Rigid_Body_2D;
}

void Physics_Module__Rigid_Body_2D__Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Physics_Module__Rigid_Body_2D* result = (Physics_Module__Rigid_Body_2D*)_product;

    result->init_physical_model();
    result->setup_base_data(coords, coords_count, collision_permissions);
    result->set_masses(masses);
    result->set_mass_multiplier(mass_multiplier);
    result->init_prev_state();
}



INIT_FIELDS(LEti::Physics_Module__Rigid_Body_2D, LEti::Dynamic_Physics_Module_2D)
FIELDS_END



Physical_Model_2D* Physics_Module__Rigid_Body_2D::M_create_physical_model() const
{
    return new Rigid_Body_Physical_Model_2D;
}



glm::vec3 Physics_Module__Rigid_Body_2D::calculate_raw_center_of_mass() const
{
    glm::vec3 result(0.0f, 0.0f, 0.0f);

    for(unsigned int i=0; i<get_physical_model()->get_polygons_count(); ++i)
    {
        const Rigid_Body_Polygon& polygon = (const Rigid_Body_Polygon&)*get_physical_model()->get_polygon(i);
        result += polygon.center_raw() * polygon.mass();
    }

    result /= ((Rigid_Body_Physical_Model_2D*)get_physical_model())->total_mass();

    return result;
}



void Physics_Module__Rigid_Body_2D::align_to_center_of_mass()
{
    if(m_on_alignment)
        m_on_alignment();

	glm::vec3 stride = -calculate_raw_center_of_mass();

	get_physical_model()->move_raw(stride);
	get_physical_model_prev_state()->update_to_current_model_state();
}



void Physics_Module__Rigid_Body_2D::set_masses(const float* _masses)
{
    Rigid_Body_Physical_Model_2D* physical_model = (Rigid_Body_Physical_Model_2D*)get_physical_model();

    physical_model->set_masses(_masses);
}



float Physics_Module__Rigid_Body_2D::mass() const
{
    Rigid_Body_Physical_Model_2D* physical_model = (Rigid_Body_Physical_Model_2D*)get_physical_model();

    return physical_model->total_mass() * m_mass_multiplier;
}

float Physics_Module__Rigid_Body_2D::moment_of_inertia() const
{
    Rigid_Body_Physical_Model_2D* physical_model = (Rigid_Body_Physical_Model_2D*)get_physical_model();

    return physical_model->moment_of_inertia() * m_mass_multiplier;
}
