#include "../../../include/Object_System/Modules/Default_Physics_Module_2D.h"

using namespace LEti;


Default_Physics_Module_2D::Default_Physics_Module_2D()
{

}

Default_Physics_Module_2D::~Default_Physics_Module_2D()
{
	delete m_physical_model_prev_state;
	delete m_physical_model;
}



void Default_Physics_Module_2D::init(const float *_coords, unsigned int _coords_count, const bool* _collision_permissions)
{
	m_can_collide = true;
	delete m_physical_model;
	m_physical_model = nullptr;
	delete m_physical_model_prev_state;
	m_physical_model_prev_state = nullptr;

	m_physical_model = new LEti::Physical_Model_2D();
	m_physical_model->setup(_coords, _coords_count, _collision_permissions);
	m_physical_model_prev_state = new LEti::Physical_Model_2D::Imprint(m_physical_model->create_imprint());
}



void Default_Physics_Module_2D::update_previous_state()
{
	L_ASSERT(!(!m_physical_model || !m_physical_model_prev_state));

	if(!m_can_collide || !m_is_dynamic) return;

	m_physical_model_prev_state->update_to_current_model_state();
}

void Default_Physics_Module_2D::update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
	L_ASSERT(!(!m_physical_model || !m_physical_model_prev_state));

	if(!m_can_collide) return;

	m_physical_model->update(_translation, _rotation, _scale);

	if(!is_dynamic()) return;

	const LEti::Geometry_2D::Rectangular_Border& prev_rb = get_physical_model_prev_state()->curr_rect_border(),
			curr_rb = get_physical_model()->curr_rect_border();

	m_dynamic_rb.left = prev_rb.left < curr_rb.left ? prev_rb.left : curr_rb.left;
	m_dynamic_rb.right = prev_rb.right > curr_rb.right ? prev_rb.right : curr_rb.right;
	m_dynamic_rb.top = prev_rb.top > curr_rb.top ? prev_rb.top : curr_rb.top;
	m_dynamic_rb.bottom = prev_rb.bottom < curr_rb.bottom ? prev_rb.bottom : curr_rb.bottom;
}



void Default_Physics_Module_2D::set_can_collide(bool _can_collide)
{
	m_can_collide = _can_collide;
}

bool Default_Physics_Module_2D::can_collide() const
{
	return m_can_collide;
}

void Default_Physics_Module_2D::set_is_dynamic(bool _is_dynamic)
{
	m_is_dynamic = _is_dynamic;
}

bool Default_Physics_Module_2D::is_dynamic() const
{
	return m_is_dynamic;
}



LEti::Physical_Model_2D* Default_Physics_Module_2D::get_physical_model()
{
	return m_physical_model;
}

LEti::Physical_Model_2D::Imprint* Default_Physics_Module_2D::get_physical_model_prev_state()
{
	return m_physical_model_prev_state;
}

const LEti::Physical_Model_2D* Default_Physics_Module_2D::get_physical_model() const
{
	return m_physical_model;
}

const LEti::Physical_Model_2D::Imprint* Default_Physics_Module_2D::get_physical_model_prev_state() const
{
	return m_physical_model_prev_state;
}

const LEti::Geometry_2D::Rectangular_Border& Default_Physics_Module_2D::get_dynamic_rb() const
{
	return m_dynamic_rb;
}
