#include "../../include/Object_System/Object_2D_Base.h"

using namespace LEti;


Object_2D_Base::Object_2D_Base() : Object_Base()
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

Object_2D_Base::~Object_2D_Base()
{

}



void Object_2D_Base::set_pos(const glm::vec3 &_position)
{
	for(unsigned int i=0; i<3; ++i)
		m_current_state.translation_matrix[3][i] = _position[i];
}

void Object_2D_Base::move(const glm::vec3 &_stride)
{
	for(unsigned int i=0; i<3; ++i)
		m_current_state.translation_matrix[3][i] += _stride[i];
}


void Object_2D_Base::set_rotation_axis(const glm::vec3 &_axis)
{
	m_current_state.rotation_axis = _axis;
	m_current_state.rotation_matrix = glm::rotate(m_current_state.rotation_angle, m_current_state.rotation_axis);
}

void Object_2D_Base::set_rotation_angle(float _angle)
{
	m_current_state.rotation_angle = _angle;
	m_current_state.rotation_matrix = glm::rotate(m_current_state.rotation_angle, m_current_state.rotation_axis);
}

void Object_2D_Base::rotate(float _angle)
{
	m_current_state.rotation_angle += _angle;
	m_current_state.rotation_matrix = glm::rotate(m_current_state.rotation_angle, m_current_state.rotation_axis);
}


void Object_2D_Base::set_scale(const glm::vec3 &_scale)
{
	for(unsigned int i=0; i<3; ++i)
		m_current_state.scale_matrix[i][i] = _scale[i];
}

void Object_2D_Base::set_scale(float _scale)
{
	for(unsigned int i=0; i<2; ++i)
		m_current_state.scale_matrix[i][i] = _scale;
}



glm::vec3 Object_2D_Base::get_pos() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_current_state.translation_matrix[3][i];
	return result;
}

glm::vec3 Object_2D_Base::get_scale() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_current_state.scale_matrix[i][i];
	return result;
}

glm::vec3 Object_2D_Base::get_rotation_axis() const
{
	return m_current_state.rotation_axis;
}

float Object_2D_Base::get_rotation_angle() const
{
	return m_current_state.rotation_angle;
}


glm::vec3 Object_2D_Base::get_pos_prev() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_previous_state.translation_matrix[3][i];
	return result;
}

glm::vec3 Object_2D_Base::get_scale_prev() const
{
	glm::vec3 result;
	for(unsigned int i=0; i<3; ++i)
		result[i] = m_previous_state.scale_matrix[i][i];
	return result;
}

glm::vec3 Object_2D_Base::get_rotation_axis_prev() const
{
	return m_previous_state.rotation_axis;
}

float Object_2D_Base::get_rotation_angle_prev() const
{
	return m_previous_state.rotation_angle;
}


glm::mat4x4 Object_2D_Base::get_translation_matrix_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

	glm::vec3 curr_pos = get_pos();
	glm::vec3 prev_pos = get_pos_prev();
	glm::vec3 diff = curr_pos - prev_pos;
	diff *= _ratio;

	glm::mat4x4 result = m_previous_state.translation_matrix;
	for(unsigned int i=0; i<3; ++i)
		result[3][i] += diff[i];
	return result;
}

glm::mat4x4 Object_2D_Base::get_rotation_matrix_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

	glm::vec3 axis_diff = m_previous_state.rotation_axis + ((m_current_state.rotation_axis - m_previous_state.rotation_axis) * _ratio);
	LEti::Math::shrink_vector_to_1(axis_diff);

	float angle_diff = m_previous_state.rotation_angle + ((m_current_state.rotation_angle - m_previous_state.rotation_angle) * _ratio);

	return glm::rotate(angle_diff, axis_diff);
}

glm::mat4x4 Object_2D_Base::get_scale_matrix_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

	glm::vec3 curr_scale = get_scale();
	glm::vec3 prev_scale = get_scale_prev();
	glm::vec3 diff_vec = curr_scale - prev_scale;
	diff_vec *= _ratio;

	glm::mat4x4 result = m_previous_state.scale_matrix;
	for(unsigned int i=0; i<3; ++i)
		result[i][i] += diff_vec[i];
	return result;
}


glm::mat4x4 Object_2D_Base::get_translation_matrix_inversed_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

	glm::vec3 curr_scale = get_scale();
	glm::vec3 prev_scale = get_scale_prev();
	glm::vec3 diff_vec = curr_scale - prev_scale;
	diff_vec *= _ratio;

	glm::mat4x4 result = m_previous_state.scale_matrix;
	for(unsigned int i=0; i<3; ++i)
		result[i][i] -= diff_vec[i];
	return result;
}

glm::mat4x4 Object_2D_Base::get_rotation_matrix_inversed_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

	glm::vec3 axis_diff = m_previous_state.rotation_axis + ((m_current_state.rotation_axis - m_previous_state.rotation_axis) * _ratio);
	LEti::Math::shrink_vector_to_1(axis_diff);

	float angle_diff = m_previous_state.rotation_angle + ((m_current_state.rotation_angle - m_previous_state.rotation_angle) * _ratio);

	return glm::rotate(angle_diff, -axis_diff);
}

glm::mat4x4 Object_2D_Base::get_scale_matrix_inversed_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

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



Object_2D_Base::Transformation_Data Object_2D_Base::get_global_transformation() const
{
	Transformation_Data result = m_current_state;

	if(m_parent)
	{
		Transformation_Data parent_global_transformation = m_parent->get_global_transformation();

		result.translation_matrix *= parent_global_transformation.translation_matrix;
		result.rotation_matrix *= parent_global_transformation.rotation_matrix;
		result.scale_matrix *= parent_global_transformation.scale_matrix;
	}

	return result;
}



bool Object_2D_Base::moved_since_last_frame() const
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



void Object_2D_Base::update_previous_state()
{
	m_previous_state = m_current_state;
}
