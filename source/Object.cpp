#include "../include/Object.h"

using namespace LEti;

void Object_Interface::set_visible(bool _visible)
{
	m_visible = _visible;
}

bool Object_Interface::get_visible() const
{
	return m_visible;
}





Drawable_Object::Drawable_Object() : Object_Interface()
{
	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	m_vertices.vertex_array = &m_vertex_array;
	m_texture.vertex_array = &m_vertex_array;

	m_translation_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	m_rotation_matrix = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_rotation_axis[0] = 0.0f;
	m_rotation_axis[1] = 1.0f;
	m_rotation_axis[2] = 0.0f;

	m_scale_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Drawable_Object::~Drawable_Object()
{
	glDeleteVertexArrays(1, &m_vertex_array);
}



glm::mat4x4 Drawable_Object::get_translation_matrix_for_time_ratio(float _ratio) const
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

glm::mat4x4 Drawable_Object::get_rotation_matrix_for_time_ratio(float _ratio) const
{
	ASSERT(_ratio < 0.0f || _ratio > 1.0f);

	glm::vec3 axis_diff = m_previous_state.rotation_axis + ((m_rotation_axis - m_previous_state.rotation_axis) * _ratio);
	float angle_diff = m_previous_state.rotation_angle + ((m_rotation_angle - m_previous_state.rotation_angle) * _ratio);

	return glm::rotate(angle_diff, axis_diff);
}

glm::mat4x4 Drawable_Object::get_scale_matrix_for_time_ratio(float _ratio) const
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



void Drawable_Object::init_texture(const char* _tex_path, const float* const tex_coords, unsigned int _tex_coords_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.init(_tex_path, tex_coords, _tex_coords_count);
	m_texture.setup_buffer(1, 2);
}

void Drawable_Object::init_vertices(const float* const _coords, unsigned int _coords_count)
{
	glBindVertexArray(m_vertex_array);
	m_vertices.load(_coords, _coords_count);
	m_vertices.setup_buffer(0, 3);
}

void Drawable_Object::init(const char* _object_name)
{
	auto tcoords = LEti::Resource_Loader::get_data<float>(_object_name, "texture_coords");
	init_texture(LEti::Resource_Loader::get_data<std::string>(_object_name, "texture_name").first->c_str(), tcoords.first, tcoords.second);

	auto coords = LEti::Resource_Loader::get_data<float>(_object_name, "coords");
	init_vertices(coords.first, coords.second);

	auto translation = LEti::Resource_Loader::get_data<float>(_object_name, "position");
	ASSERT(translation.second != 3);
	set_pos(translation.first[0], translation.first[1], translation.first[2]);

	auto scale = LEti::Resource_Loader::get_data<float>(_object_name, "scale");
	ASSERT(scale.second != 3);
	set_scale(scale.first[0], scale.first[1], scale.first[2]);

	auto raxis = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_axis");
	ASSERT(raxis.second != 3);
	set_rotation_axis(raxis.first[0], raxis.first[1], raxis.first[2]);

	auto rangle = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_angle");
	ASSERT(rangle.second != 1);
	set_rotation_angle(*rangle.first);
}



LEti::Vertices& Drawable_Object::get_vertices()
{
	return m_vertices;
}

LEti::Texture& Drawable_Object::get_texture()
{
	return m_texture;
}



void Drawable_Object::set_texture(const char* _name)
{
	glBindVertexArray(m_vertex_array);
	m_texture.set_picture(_name);
}

void Drawable_Object::set_texture_coords(const float* _tc, unsigned int _tc_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.set_texture_coords(_tc, _tc_count);
	m_texture.setup_buffer(1, 2);
}



void Drawable_Object::update_previous_state()
{
	m_previous_state.translation_matrix = m_translation_matrix;
	m_previous_state.rotation_axis = m_rotation_axis;
	m_previous_state.rotation_angle = m_rotation_angle;
	m_previous_state.rotation_matrix = m_rotation_matrix;
	m_previous_state.scale_matrix = m_scale_matrix;
}



void Drawable_Object::draw() const
{
	if (!get_visible()) return;

	ASSERT(m_vertex_array == 0 || m_vertices.get_vertices_count() == 0 || m_texture.size() == 0);

	glm::mat4x4 result_matrix = m_translation_matrix * m_rotation_matrix * m_scale_matrix;
	LEti::Shader::set_transform_matrix(result_matrix);

	glBindVertexArray(m_vertex_array);

	LEti::Shader::set_texture(m_texture);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.get_vertices_count());

	glBindVertexArray(0);
}

void Drawable_Object::update()
{

}



void Drawable_Object::set_pos(float _x, float _y, float _z)
{
	m_translation_matrix[3][0] = _x;
	m_translation_matrix[3][1] = _y;
	m_translation_matrix[3][2] = _z;
}

void Drawable_Object::move(float _x, float _y, float _z)
{
	m_translation_matrix[3][0] += _x;
	m_translation_matrix[3][1] += _y;
	m_translation_matrix[3][2] += _z;
}


void Drawable_Object::set_rotation_axis(float _x, float _y, float _z)
{
	m_rotation_axis[0] = _x;
	m_rotation_axis[1] = _y;
	m_rotation_axis[2] = _z;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}

void Drawable_Object::set_rotation_angle(float _angle)
{
	m_rotation_angle = _angle;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}

void Drawable_Object::set_rotation_data(float _axis_x, float _axis_y, float _axis_z, float _angle)
{
	m_rotation_axis[0] = _axis_x;
	m_rotation_axis[1] = _axis_y;
	m_rotation_axis[2] = _axis_z;
	m_rotation_angle = _angle;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}

void Drawable_Object::rotate(float _angle)
{
	m_rotation_angle += _angle;

	if (m_rotation_angle >= 6.28318f)
		m_rotation_angle -= 6.28318f;
	if (m_rotation_angle <= -6.28318f)
		m_rotation_angle += -6.28318f;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}


void Drawable_Object::set_scale(float _scale_x, float _scale_y, float _scale_z)
{
	m_scale_matrix[0][0] = _scale_x;
	m_scale_matrix[1][1] = _scale_y;
	m_scale_matrix[2][2] = _scale_z;
}

void Drawable_Object::set_overall_scale(float _scale)
{
	Drawable_Object::set_scale(_scale, _scale, _scale);
}



glm::vec3 Drawable_Object::get_pos() const
{
	glm::vec3 result;
	for (unsigned int i = 0; i < 3; ++i)
		result[i] = m_translation_matrix[3][i];
	return result;
}

glm::vec3 Drawable_Object::get_scale() const
{
	glm::vec3 result;
	for (unsigned int i = 0; i < 3; ++i)
		result[i] = m_scale_matrix[i][i];
	return result;
}

glm::vec3 Drawable_Object::get_rotation_axis() const
{
	return m_rotation_axis;
}

float Drawable_Object::get_rotation_angle() const
{
	return m_rotation_angle;
}


glm::vec3 Drawable_Object::get_pos_prev() const
{
	glm::vec3 result;
	for (unsigned int i = 0; i < 3; ++i)
		result[i] = m_previous_state.translation_matrix[3][i];
	return result;
}

glm::vec3 Drawable_Object::get_scale_prev() const
{
	glm::vec3 result;
	for (unsigned int i = 0; i < 3; ++i)
		result[i] = m_previous_state.scale_matrix[i][i];
	return result;
}

glm::vec3 Drawable_Object::get_rotation_axis_prev() const
{
	return m_previous_state.rotation_axis;
}

float Drawable_Object::get_rotation_angle_prev() const
{
	return m_previous_state.rotation_angle;
}











Object_2D::Object_2D()
{

}

Object_2D::~Object_2D()
{
	delete m_physical_model;
	delete m_physical_model_prev_state;
}


void Object_2D::init_physical_model(const float *_coords, unsigned int _coords_count)
{
	m_can_cause_collision = true;
	delete m_physical_model;
	m_physical_model = nullptr;
	delete m_physical_model_prev_state;
	m_physical_model_prev_state = nullptr;

	m_physical_model = new Physical_Model_2D(_coords, _coords_count);
	m_physical_model->update(m_translation_matrix, m_rotation_matrix, m_scale_matrix);
}


void Object_2D::init(const char* _object_name)
{
	Drawable_Object::init(_object_name);

	std::pair<const float*, unsigned int> physical_model_data;
	LEti::Resource_Loader::assign(physical_model_data, _object_name, "physical_model_data");
	if((physical_model_data.first))
		init_physical_model(physical_model_data.first, physical_model_data.second);
}



void Object_2D::set_dynamic(bool _is_dynamic)
{
	if(_is_dynamic && !m_is_dynamic)
	{
		m_physical_model_prev_state = new Physical_Model_2D::Imprint(m_physical_model->create_imprint());
	}
	else if(!_is_dynamic && m_is_dynamic)
	{
		delete m_physical_model_prev_state;
		m_physical_model_prev_state = nullptr;
	}
	m_is_dynamic = _is_dynamic;
}

bool Object_2D::is_dynamic() const
{
	return m_is_dynamic;
}


void Object_2D::set_can_cause_collision(bool _can_cause_collision)
{
	m_can_cause_collision = _can_cause_collision;
}

bool Object_2D::get_collision_possibility() const
{
	return m_can_cause_collision;
}



void Object_2D::draw() const
{
	LEti::Camera::use_2d();
    glDisable(GL_DEPTH_TEST);

	Drawable_Object::draw();
}

void Object_2D::update()
{
	Drawable_Object::update();
	if(m_can_cause_collision)
	{
		if(is_dynamic())
		{
			get_physical_model_prev_state()->update_to_current_model_state();
			m_physical_model->update(m_translation_matrix, m_rotation_matrix, m_scale_matrix);

			const Physical_Model_2D::Rectangular_Border& prev_rb = get_physical_model_prev_state()->curr_rect_border(),
														 curr_rb = get_physical_model()->curr_rect_border();

			m_dynamic_rb.left = prev_rb.left < curr_rb.left ? prev_rb.left : curr_rb.left;
			m_dynamic_rb.right = prev_rb.right > curr_rb.right ? prev_rb.right : curr_rb.right;
			m_dynamic_rb.top = prev_rb.top > curr_rb.top ? prev_rb.top : curr_rb.top;
			m_dynamic_rb.bottom = prev_rb.bottom < curr_rb.bottom ? prev_rb.bottom : curr_rb.bottom;
		}
		else
		{
			m_physical_model->update(m_translation_matrix, m_rotation_matrix, m_scale_matrix);
		}
	}
}



Physical_Model_2D* Object_2D::get_physical_model()
{
	return m_physical_model;
}

Physical_Model_2D::Imprint* Object_2D::get_physical_model_prev_state()
{
	return m_physical_model_prev_state;
}


const Physical_Model_2D* Object_2D::get_physical_model() const
{
	return m_physical_model;
}

const Physical_Model_2D::Imprint* Object_2D::get_physical_model_prev_state() const
{
	return m_physical_model_prev_state;
}

const Physical_Model_2D::Rectangular_Border& Object_2D::get_dynamic_rb() const
{
	return m_dynamic_rb;
}



Geometry::Intersection_Data Object_2D::get_precise_time_ratio_of_collision(const Object_2D& _other, float _min_ratio, float _max_ratio) const
{
	ASSERT(!m_can_cause_collision || !_other.m_can_cause_collision);
	ASSERT(!is_dynamic() && !_other.is_dynamic());
	ASSERT(_min_ratio < 0.0f || _max_ratio < 0.0f || _min_ratio > 1.0f || _max_ratio > 1.0f);

	/*float time_mid_point = (_max_ratio + _min_ratio) / 2.0f;

	Geometry::Intersection_Data result;

	if(_level < m_precision_level)
	{
		result = get_precise_time_ratio_of_collision(_level + 1, _other, _min_ratio, time_mid_point);
		if(result)
			return result;

		result = get_precise_time_ratio_of_collision(_level + 1, _other, time_mid_point, _max_ratio);
		return result;
	}

	//if this is the maximum precision level
	Physical_Model_2D this_init_pm = *get_physical_model_prev_state();
	Physical_Model_2D other_init_pm = *_other.get_physical_model_prev_state();

	glm::vec3 this_pos = get_pos();
	glm::vec3 this_pos_prev = get_pos_prev();
	glm::vec3 this_stride = this_pos - this_pos_prev;

	glm::mat4x4 this_stride_matrix = m_previous_state.translation_matrix;
	for(unsigned int i=0; i<3; ++i)
		this_stride_matrix[3][i] = this_pos_prev[i] + this_stride[i];

	glm::vec3 other_pos = _other.get_pos();
	glm::vec3 other_pos_prev = _other.get_pos_prev();
	glm::vec3 other_stride = other_pos - other_pos_prev;

	glm::mat4x4 other_stride_matrix = m_previous_state.translation_matrix;
	for(unsigned int i=0; i<3; ++i)
		other_stride_matrix[3][i] = other_pos_prev[i] + other_stride[i];

	this_init_pm.update(this_stride_matrix, m_previous_state.rotation_matrix, m_previous_state.scale_matrix);
	other_init_pm.update(other_stride_matrix, _other.m_previous_state.rotation_matrix, _other.m_previous_state.scale_matrix);

	result = this_init_pm.is_intersecting_with_another_model(other_init_pm);
	if(!result)
		return result;
	result.time_of_intersection_ratio = time_mid_point;
	return result;*/

	float diff = _max_ratio - _min_ratio;
	float step_diff = diff / (float)m_precision_level;

	float curr_time_point = _min_ratio;
	while(curr_time_point <= _max_ratio)
	{
		Physical_Model_2D::Imprint this_impr = *m_physical_model_prev_state;

		glm::mat4x4 this_tmftr = get_translation_matrix_for_time_ratio(curr_time_point);
		glm::mat4x4 other_tmftr = _other.get_translation_matrix_for_time_ratio(curr_time_point);

		this_impr.update(get_translation_matrix_for_time_ratio(curr_time_point), get_rotation_matrix_for_time_ratio(curr_time_point), get_scale_matrix_for_time_ratio(curr_time_point));
		Physical_Model_2D::Imprint other_impr = *_other.m_physical_model_prev_state;
		other_impr.update(_other.get_translation_matrix_for_time_ratio(curr_time_point), _other.get_rotation_matrix_for_time_ratio(curr_time_point), _other.get_scale_matrix_for_time_ratio(curr_time_point));
		Geometry::Intersection_Data id = this_impr.imprints_intersect(other_impr);
		if(id) { id.time_of_intersection_ratio = Math::floats_are_equal(curr_time_point, _min_ratio) ? curr_time_point : curr_time_point - step_diff; return id; }

		curr_time_point += step_diff;
	}

	//minimal ratio check
//	if(_level == 0)
//	{
//		Physical_Model_2D::Imprint this_mrc = *m_physical_model_prev_state;
//		this_mrc.update(get_translation_matrix_for_time_ratio(_min_ratio), get_rotation_matrix_for_time_ratio(_min_ratio), get_scale_matrix_for_time_ratio(_min_ratio));
//		Physical_Model_2D::Imprint other_mrc = *_other.m_physical_model_prev_state;
//		other_mrc.update(_other.get_translation_matrix_for_time_ratio(_min_ratio), _other.get_rotation_matrix_for_time_ratio(_min_ratio), _other.get_scale_matrix_for_time_ratio(_min_ratio));
//		Geometry::Intersection_Data id = this_mrc.imprints_intersect(other_mrc);
//		if(id) { id.time_of_intersection_ratio = _min_ratio; return id; }
//	}

	return Geometry::Intersection_Data();
}



Geometry::Intersection_Data Object_2D::is_colliding_with_other(const Object_2D& _other) const
{
	ASSERT(m_can_cause_collision && !m_physical_model);
	ASSERT(_other.m_can_cause_collision && !_other.m_physical_model);
	if(!_other.is_dynamic() && !is_dynamic())
	{
		if(!_other.m_can_cause_collision || !m_can_cause_collision)
			return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
		return m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
	}

	//	TODO: remove this - only for testing purpose
	auto really_colliding = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
	//

	const Physical_Model_2D::Rectangular_Border& this_rb_prev = get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& this_rb_curr = get_physical_model()->curr_rect_border();

	const Physical_Model_2D::Rectangular_Border& other_rb_prev = _other.get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& other_rb_curr = _other.get_physical_model()->curr_rect_border();

	std::pair<glm::vec3, glm::vec3> this_segments[4] = {
		{{this_rb_prev.left, this_rb_prev.top, 0.0f}, {this_rb_curr.left, this_rb_curr.top, 0.0f}},
		{{this_rb_prev.right, this_rb_prev.top, 0.0f}, {this_rb_curr.right, this_rb_curr.top, 0.0f}},
		{{this_rb_prev.left, this_rb_prev.bottom, 0.0f}, {this_rb_curr.left, this_rb_curr.bottom, 0.0f}},
		{{this_rb_prev.right, this_rb_prev.bottom, 0.0f}, {this_rb_curr.right, this_rb_curr.bottom, 0.0f}}
	};
	std::pair<glm::vec3, glm::vec3> other_segments[4] = {
		{{other_rb_prev.left, other_rb_prev.top, 0.0f}, {other_rb_curr.left, other_rb_curr.top, 0.0f}},
		{{other_rb_prev.right, other_rb_prev.top, 0.0f}, {other_rb_curr.right, other_rb_curr.top, 0.0f}},
		{{other_rb_prev.left, other_rb_prev.bottom, 0.0f}, {other_rb_curr.left, other_rb_curr.bottom, 0.0f}},
		{{other_rb_prev.right, other_rb_prev.bottom, 0.0f}, {other_rb_curr.right, other_rb_curr.bottom, 0.0f}}
	};

	float this_segments_lengths[4] = {
		Math::get_distance(this_segments[0].first, this_segments[0].second),
		Math::get_distance(this_segments[1].first, this_segments[1].second),
		Math::get_distance(this_segments[2].first, this_segments[2].second),
		Math::get_distance(this_segments[3].first, this_segments[3].second)
	};
	bool this_frame_is_static = true;
	for(unsigned int i=0; i<4; ++i)
	{
		if(!Math::floats_are_equal(this_segments_lengths[i], 0.0f))
		{
			this_frame_is_static = false;
			break;
		}
	}

	float other_segments_lengths[4] = {
		Math::get_distance(other_segments[0].first, other_segments[0].second),
		Math::get_distance(other_segments[1].first, other_segments[1].second),
		Math::get_distance(other_segments[2].first, other_segments[2].second),
		Math::get_distance(other_segments[3].first, other_segments[3].second)
	};
	bool other_frame_is_static = true;
	for(unsigned int i=0; i<4; ++i)
	{
		if(!Math::floats_are_equal(other_segments_lengths[i], 0.0f))
		{
			other_frame_is_static = false;
			break;
		}
	}

	float min_ratio = 1.1f, max_ratio = -0.1f;

	//	case in which both objects are moving and their paths intersects
	if(!this_frame_is_static && !other_frame_is_static /*&& really_colliding*/)
	{
		for(unsigned int i=0; i<4; ++i)
		{
			for(unsigned int j=0; j<4; ++j)
			{
				Geometry::Intersection_Data id =
						Geometry_2D::segments_intersect(this_segments[i].first, this_segments[i].second,
														other_segments[j].first, other_segments[j].second);
				if(id)
				{
					if(!Math::floats_are_equal(this_segments_lengths[i], 0.0f))
					{
						float this_ratio = Math::get_distance(this_segments[i].first, id.point) / this_segments_lengths[i];
						if(min_ratio > this_ratio) min_ratio = this_ratio;
						if(max_ratio < this_ratio) max_ratio = this_ratio;
					}
					if(!Math::floats_are_equal(other_segments_lengths[i], 0.0f))
					{
						float other_ratio = Math::get_distance(other_segments[j].first, id.point) / other_segments_lengths[j];
						if(min_ratio > other_ratio) min_ratio = other_ratio;
						if(max_ratio < other_ratio) max_ratio = other_ratio;
					}
				}
			}
		}
	}
	if(max_ratio > 0.0f && max_ratio <= 1.0f && min_ratio < 1.0f && min_ratio >= 0.0f)
	{
		if(Math::floats_are_equal(min_ratio, max_ratio))
		{
			std::cout << min_ratio << ' ' << max_ratio << "\n";
			Geometry::Intersection_Data result = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
			result.time_of_intersection_ratio = min_ratio;
			return result;
		}
		else
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_other, min_ratio, max_ratio);
			if(result) std::cout << "detected collision! precise collision time ratio: " << result.time_of_intersection_ratio << '\n';
			return result;
		}

	}
//	std::cout << min_ratio << ' ' << max_ratio << "\n";

	auto is_in_bounds_of_segment_by_x = [](const std::pair<glm::vec3, glm::vec3>& _segment, const glm::vec3& _point)->bool{
		bool is_first_on_the_left = _segment.first.x < _segment.second.x;
		return is_first_on_the_left ? _segment.first.x <= _point.x && _segment.second.x >= _point.x
									: _segment.second.x <= _point.x && _segment.first.x >= _point.x;
	};

	//	case in which both objects moves, so they may intersect, but their frames' angles' paths does not intersect
	if(!this_frame_is_static && !other_frame_is_static && really_colliding)
	{
		bool this_on_the_left = m_dynamic_rb.left < _other.m_dynamic_rb.left;
		float left_of_shared_space = this_on_the_left ? _other.m_dynamic_rb.left : m_dynamic_rb.left;
		float right_of_shared_space = this_on_the_left ? m_dynamic_rb.right : _other.m_dynamic_rb.right;

		float temp_ratio_1 = -0.1f, temp_ratio_2 = -0.1f;

		if(this_on_the_left)
		{
			temp_ratio_1 = (left_of_shared_space - m_dynamic_rb.left) / (m_dynamic_rb.right - m_dynamic_rb.left);
			temp_ratio_2 = (_other.m_dynamic_rb.right - right_of_shared_space) / (_other.m_dynamic_rb.right - _other.m_dynamic_rb.left);
		}
		else
		{
			temp_ratio_1 = (left_of_shared_space - _other.m_dynamic_rb.left) / (_other.m_dynamic_rb.right - _other.m_dynamic_rb.left);
			temp_ratio_2 = (m_dynamic_rb.right - right_of_shared_space) / (m_dynamic_rb.right - m_dynamic_rb.left);
		}

//		if(temp_ratio_1 > temp_ratio_2)
//		{
//			float temp = temp_ratio_1;
//			temp_ratio_1 = temp_ratio_2;
//			temp_ratio_2 = temp;
//		}

//		if(local_ratio_1 >= 0.0f && local_ratio_1 <= 1.0f && local_ratio_1 < min_ratio
//				&& local_ratio_2 >= 0.0f && local_ratio_2 <= 1.0f && local_ratio_2 > max_ratio)
//		{
//			min_ratio = local_ratio_1;
//			max_ratio = local_ratio_2;
//		}
		if(temp_ratio_1 >= 0.0f && temp_ratio_1 <= 1.0f)
		{
			if(min_ratio > temp_ratio_1)
				min_ratio = temp_ratio_1;
			if(max_ratio < temp_ratio_1)
				max_ratio = temp_ratio_1;
		}
		if(temp_ratio_2 >= 0.0f && temp_ratio_2 <= 1.0f)
		{
			if(min_ratio > temp_ratio_2)
				min_ratio = temp_ratio_2;
			if(max_ratio < temp_ratio_2)
				max_ratio = temp_ratio_2;
		}
	}
	if(max_ratio > 0.0f && max_ratio <= 1.0f && min_ratio < 1.0f && min_ratio >= 0.0f)
	{
		if(Math::floats_are_equal(min_ratio, max_ratio))
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_other, min_ratio, 1.0f);
			result.time_of_intersection_ratio = min_ratio;
			if(!result)
				std::cout << "shit\n";
			return result;
		}
		else
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_other, min_ratio, max_ratio);
			if(result) std::cout << "detected collision! precise collision time ratio: " << result.time_of_intersection_ratio << '\n';
			return result;
		}
	}
	else
	{
		if(!this_frame_is_static && !other_frame_is_static && really_colliding)
			std::cout << "(2 are moving) collision should be here, but it's not detected!\n";
	}

	//case in which one of the objects is static and other is dynamic
	bool only_one_frame_is_static = (this_frame_is_static && !other_frame_is_static) || (!this_frame_is_static && other_frame_is_static);
	if(only_one_frame_is_static /*&& really_colliding*/)
	{
		const Object_2D& static_object = this_frame_is_static ? *this : _other;
		const Object_2D& dynamic_object = other_frame_is_static ? *this : _other;
		const std::pair<glm::vec3, glm::vec3>* const dynamic_object_segments = this_frame_is_static ? other_segments : this_segments;
		const float* const dynamic_object_segments_lengths = this_frame_is_static ? other_segments_lengths : this_segments_lengths;

		auto test = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());

		for(unsigned int i=0; i<4; ++i)
		{
			const std::pair<glm::vec3, glm::vec3>& curr_segment = dynamic_object_segments[i];
			float curr_segment_length = dynamic_object_segments_lengths[i];
			const Physical_Model_2D::Rectangular_Border& rb = static_object.get_physical_model()->curr_rect_border();

			float temp_ratio_1 = 1.1f, temp_ratio_2 = -0.1f;

			Geometry_2D::Equasion_Data eq(curr_segment.first, curr_segment.second);
			if(eq.is_vertical())
			{
				if(Math::floats_are_equal(eq.get_x_if_vertical(), rb.left), Math::floats_are_equal(eq.get_x_if_vertical(), rb.right))
				{
					min_ratio = 0.0f;	//maybe not quite right - ratio may be 1.0f
					max_ratio = 0.0f;
					continue;
				}
			}

			temp_ratio_1 = Math::get_distance({rb.left, eq.solve_by_x(rb.left), 0.0f}, curr_segment.first) / curr_segment_length;
			temp_ratio_2 = Math::get_distance({rb.right, eq.solve_by_x(rb.right), 0.0f}, curr_segment.first) / curr_segment_length;

//			if(temp_ratio_1 > temp_ratio_2)
//			{
//				float temp = temp_ratio_1;
//				temp_ratio_1 = temp_ratio_2;
//				temp_ratio_2 = temp;
//			}

			if(temp_ratio_1 >= 0.0f && temp_ratio_1 <= 1.0f)
			{
				if(min_ratio > temp_ratio_1)
					min_ratio = temp_ratio_1;
				if(max_ratio < temp_ratio_1)
					max_ratio = temp_ratio_1;
			}
			if(temp_ratio_2 >= 0.0f && temp_ratio_2 <= 1.0f)
			{
				if(min_ratio > temp_ratio_2)
					min_ratio = temp_ratio_2;
				if(max_ratio < temp_ratio_2)
					max_ratio = temp_ratio_2;
			}
		}
	}
	if(max_ratio > 0.0f && max_ratio <= 1.0f && min_ratio < 1.0f && min_ratio >= 0.0f)
	{
		if(Math::floats_are_equal(min_ratio, max_ratio))
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_other, min_ratio, 1.0f);

			result.time_of_intersection_ratio = min_ratio;
			if(!result)
				std::cout << "shit\n";
			return result;
		}
		else
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_other, min_ratio, max_ratio);
			if(result) std::cout << "detected collision! precise collision time ratio: " << result.time_of_intersection_ratio << '\n';
			if(!result)
			{
				std::cout << "shit\n";
				Geometry::Intersection_Data shit = get_precise_time_ratio_of_collision(_other, min_ratio, max_ratio);
			}
			return result;
		}


	}

	if(really_colliding && this_segments_lengths[0] > 0.0f && other_segments_lengths[0] > 0.0f)
	{
//		std::cout << "(2 are moving) collision should be here, but it's not detected!\n";
	}
	if(really_colliding && ((Math::floats_are_equal(this_segments_lengths[0], 0.0f) && other_segments_lengths[0] > 0.0f) || (Math::floats_are_equal(this_segments_lengths[0], 0.0f) && other_segments_lengths[0] > 0.0f)))
	{
		std::cout << "(1 is moving) collision should be here, but it's not detected!\n";
	}

	return really_colliding;
}









/*
Object_3D::Object_3D() : Colliding_Object()
{

}

Object_3D::~Object_3D()
{

}


void Object_3D::init_physical_model(const float *_coords, unsigned int _coords_count)
{
	Colliding_Object::init_physical_model(_coords, _coords_count);
    m_physical_model = new Physical_Model_3D(_coords, _coords_count);
}


void Object_3D::init(const char* _object_name)
{
	Colliding_Object::init(_object_name);
}



void Object_3D::draw() const
{
	LEti::Camera::use_3d();
    glEnable(GL_DEPTH_TEST);

	Colliding_Object::draw();
}

void Object_3D::update()
{
	Colliding_Object::update();
}
*/
