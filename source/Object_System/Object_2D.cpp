#include <Object_System/Object_2D.h>

using namespace LEti;


Object_2D::Object_2D() : Object_Base()
{
	m_current_state.translation_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	m_current_state.rotation_matrix = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_current_state.rotation_axis[0] = 0.0f;
	m_current_state.rotation_axis[1] = 1.0f;
	m_current_state.rotation_axis[2] = 0.0f;

	m_current_state.scale_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	m_previous_state = m_current_state;
}

Object_2D::~Object_2D()
{
	remove_draw_module();
	remove_physics_module();
}



void Object_2D::set_pos(const glm::vec3 &_position)
{
	for(unsigned int i=0; i<3; ++i)
		m_current_state.translation_matrix[3][i] = _position[i];
}

void Object_2D::move(const glm::vec3 &_stride)
{
	for(unsigned int i=0; i<3; ++i)
		m_current_state.translation_matrix[3][i] += _stride[i];
}


void Object_2D::set_rotation_axis(const glm::vec3 &_axis)
{
	m_current_state.rotation_axis = _axis;
	m_current_state.rotation_matrix = glm::rotate(m_current_state.rotation_angle, m_current_state.rotation_axis);
}

void Object_2D::set_rotation_angle(float _angle)
{
	m_current_state.rotation_angle = _angle;
	m_current_state.rotation_matrix = glm::rotate(m_current_state.rotation_angle, m_current_state.rotation_axis);
}

void Object_2D::rotate(float _angle)
{
	m_current_state.rotation_angle += _angle;
	m_current_state.rotation_matrix = glm::rotate(m_current_state.rotation_angle, m_current_state.rotation_axis);
}


void Object_2D::set_scale(const glm::vec3 &_scale)
{
	for(unsigned int i=0; i<3; ++i)
		m_current_state.scale_matrix[i][i] = _scale[i];
}

void Object_2D::set_scale(float _scale)
{
	for(unsigned int i=0; i<2; ++i)
		m_current_state.scale_matrix[i][i] = _scale;
}



glm::vec3 Object_2D::get_pos() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_current_state.translation_matrix[3][i];
	return result;
}

glm::vec3 Object_2D::get_scale() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_current_state.scale_matrix[i][i];
	return result;
}

glm::vec3 Object_2D::get_rotation_axis() const
{
	return m_current_state.rotation_axis;
}

float Object_2D::get_rotation_angle() const
{
	return m_current_state.rotation_angle;
}


glm::vec3 Object_2D::get_pos_prev() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_previous_state.translation_matrix[3][i];
	return result;
}

glm::vec3 Object_2D::get_scale_prev() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_previous_state.scale_matrix[i][i];
	return result;
}

glm::vec3 Object_2D::get_rotation_axis_prev() const
{
	return m_previous_state.rotation_axis;
}

float Object_2D::get_rotation_angle_prev() const
{
	return m_previous_state.rotation_angle;
}


glm::mat4x4 Object_2D::get_translation_matrix_for_time_ratio(float _ratio) const
{
	L_ASSERT(!(_ratio < 0.0f || _ratio > 1.0f));

	glm::vec3 curr_pos = get_pos();
	glm::vec3 prev_pos = get_pos_prev();
	glm::vec3 diff = curr_pos - prev_pos;
	diff *= _ratio;

	glm::mat4x4 result = m_previous_state.translation_matrix;
	for(unsigned int i=0; i<3; ++i)
		result[3][i] += diff[i];
	return result;
}

glm::mat4x4 Object_2D::get_rotation_matrix_for_time_ratio(float _ratio) const
{
	L_ASSERT(!(_ratio < 0.0f || _ratio > 1.0f));

	glm::vec3 axis_diff = m_previous_state.rotation_axis + ((m_current_state.rotation_axis - m_previous_state.rotation_axis) * _ratio);
	LEti::Math::shrink_vector_to_1(axis_diff);

	float angle_diff = m_previous_state.rotation_angle + ((m_current_state.rotation_angle - m_previous_state.rotation_angle) * _ratio);

	return glm::rotate(angle_diff, axis_diff);
}

glm::mat4x4 Object_2D::get_scale_matrix_for_time_ratio(float _ratio) const
{
	L_ASSERT(!(_ratio < 0.0f || _ratio > 1.0f));

	glm::vec3 curr_scale = get_scale();
	glm::vec3 prev_scale = get_scale_prev();
	glm::vec3 diff_vec = curr_scale - prev_scale;
	diff_vec *= _ratio;

	glm::mat4x4 result = m_previous_state.scale_matrix;
	for(unsigned int i=0; i<3; ++i)
		result[i][i] += diff_vec[i];
	return result;
}


glm::mat4x4 Object_2D::get_translation_matrix_inversed_for_time_ratio(float _ratio) const
{
	L_ASSERT(!(_ratio < 0.0f || _ratio > 1.0f));

	glm::vec3 curr_scale = get_scale();
	glm::vec3 prev_scale = get_scale_prev();
	glm::vec3 diff_vec = curr_scale - prev_scale;
	diff_vec *= _ratio;

	glm::mat4x4 result = m_previous_state.scale_matrix;
	for(unsigned int i=0; i<3; ++i)
		result[i][i] -= diff_vec[i];
	return result;
}

glm::mat4x4 Object_2D::get_rotation_matrix_inversed_for_time_ratio(float _ratio) const
{
	L_ASSERT(!(_ratio < 0.0f || _ratio > 1.0f));

	glm::vec3 axis_diff = m_previous_state.rotation_axis + ((m_current_state.rotation_axis - m_previous_state.rotation_axis) * _ratio);
	LEti::Math::shrink_vector_to_1(axis_diff);

	float angle_diff = m_previous_state.rotation_angle + ((m_current_state.rotation_angle - m_previous_state.rotation_angle) * _ratio);

	return glm::rotate(angle_diff, -axis_diff);
}

glm::mat4x4 Object_2D::get_scale_matrix_inversed_for_time_ratio(float _ratio) const
{
	L_ASSERT(!(_ratio < 0.0f || _ratio > 1.0f));

	glm::vec3 curr_scale = get_scale();
	glm::vec3 prev_scale = get_scale_prev();
	glm::vec3 diff_vec = curr_scale - prev_scale;
	diff_vec *= _ratio;

	glm::mat4x4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	for(unsigned int i=0; i<3; ++i)
		result[i][i] -= diff_vec[i];
	return result;
}



bool Object_2D::moved_since_last_frame() const
{
	for(unsigned int x=0; x<4; ++x)
	{
		for(unsigned int y=0; y<4; ++y)
		{
			if(!LEti::Math::floats_are_equal(m_current_state.translation_matrix[x][y], m_previous_state.translation_matrix[x][y]))
				return true;
			if(!LEti::Math::floats_are_equal(m_current_state.rotation_matrix[x][y], m_previous_state.rotation_matrix[x][y]))
				return true;
			if(!LEti::Math::floats_are_equal(m_current_state.scale_matrix[x][y], m_previous_state.scale_matrix[x][y]))
				return true;
		}
	}
	return false;
}



void Object_2D::init(const char *_object_name)
{
	remove_draw_module();
	remove_physics_module();

	auto translation = LEti::Resource_Loader::get_data<float>(_object_name, "position");
	L_ASSERT(!(translation.second != 3));
	set_pos({translation.first[0], translation.first[1], translation.first[2]});

	auto scale = LEti::Resource_Loader::get_data<float>(_object_name, "scale");
	L_ASSERT(!(scale.second != 3));
	set_scale({scale.first[0], scale.first[1], scale.first[2]});

	auto raxis = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_axis");
	L_ASSERT(!(raxis.second != 3));
	set_rotation_axis({raxis.first[0], raxis.first[1], raxis.first[2]});

	auto rangle = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_angle");
	L_ASSERT(!(rangle.second != 1));
	set_rotation_angle(*rangle.first);

	std::pair<const float*, unsigned int> tcoords;
	LEti::Resource_Loader::assign<float>(tcoords, _object_name, "texture_coords");

	if(tcoords.first)
	{
		create_draw_module();
		m_draw_module->init_texture(LEti::Resource_Loader::get_data<std::string>(_object_name, "texture_name").first->c_str(), tcoords.first, tcoords.second);
		auto coords = LEti::Resource_Loader::get_data<float>(_object_name, "coords");
		m_draw_module->init_vertices(coords.first, coords.second);
	}

	std::pair<const float*, unsigned int> physical_model_data;
	LEti::Resource_Loader::assign(physical_model_data, _object_name, "physical_model_data");

	if((physical_model_data.first))
	{
		create_physics_module();
		m_physics_module->init(physical_model_data.first, physical_model_data.second);
	}
}



void Object_2D::create_draw_module()
{
	remove_draw_module();
	m_draw_module = new Default_Draw_Module_2D;
}

void Object_2D::remove_draw_module()
{
	delete m_draw_module;
	m_draw_module = nullptr;
}

Default_Draw_Module_2D* Object_2D::draw_module()
{
	return m_draw_module;
}

const Default_Draw_Module_2D* Object_2D::draw_module() const
{
	return m_draw_module;
}


void Object_2D::create_physics_module()
{
	remove_physics_module();
	m_physics_module = new Default_Physics_Module_2D;
}

void Object_2D::remove_physics_module()
{
	delete m_physics_module;
	m_physics_module = nullptr;
}

Default_Physics_Module_2D* Object_2D::physics_module()
{
	return m_physics_module;
}

const Default_Physics_Module_2D* Object_2D::physics_module() const
{
	return m_physics_module;
}



void Object_2D::revert_to_previous_state()
{
	m_current_state = m_previous_state;
}

void Object_2D::update_previous_state()
{
	m_previous_state = m_current_state;

	if(!m_physics_module) return;

	m_physics_module->update_previous_state();
}

void Object_2D::update(float _ratio)
{
	if(!m_physics_module) return;

	m_physics_module->update(m_current_state.translation_matrix, m_current_state.rotation_matrix, m_current_state.scale_matrix);
}

void Object_2D::draw() const
{
	if(!m_draw_module) return;

	m_draw_module->draw(m_current_state.translation_matrix, m_current_state.rotation_matrix, m_current_state.scale_matrix);
}
