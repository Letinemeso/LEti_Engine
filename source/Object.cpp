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



void Drawable_Object::set_pos(float _x, float _y, float _z)
{
	m_previous_state.translation_matrix = m_translation_matrix;

	m_translation_matrix[3][0] = _x;
	m_translation_matrix[3][1] = _y;
	m_translation_matrix[3][2] = _z;
}

void Drawable_Object::move(float _x, float _y, float _z)
{
	m_previous_state.translation_matrix = m_translation_matrix;

	m_translation_matrix[3][0] += _x;
	m_translation_matrix[3][1] += _y;
	m_translation_matrix[3][2] += _z;
}


void Drawable_Object::set_rotation_axis(float _x, float _y, float _z)
{
	m_previous_state.rotation_axis = m_rotation_axis;

	m_rotation_axis[0] = _x;
	m_rotation_axis[1] = _y;
	m_rotation_axis[2] = _z;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}

void Drawable_Object::set_rotation_angle(float _angle)
{
	m_previous_state.rotation_angle = m_rotation_angle;

	while (_angle >= 6.28318f)
		_angle -= 6.28318f;
	while (_angle <= -6.28318f)
		_angle += 6.28318f;

	m_rotation_angle = _angle;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}

void Drawable_Object::set_rotation_data(float _axis_x, float _axis_y, float _axis_z, float _angle)
{
	m_previous_state.rotation_angle = m_rotation_angle;
	m_previous_state.rotation_axis = m_rotation_axis;

	m_rotation_axis[0] = _axis_x;
	m_rotation_axis[1] = _axis_y;
	m_rotation_axis[2] = _axis_z;
	m_rotation_angle = _angle;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}

void Drawable_Object::rotate(float _angle)
{
	m_previous_state.rotation_angle = m_rotation_angle;

	m_rotation_angle += _angle;

	if (m_rotation_angle >= 6.28318f)
		m_rotation_angle -= 6.28318f;
	if (m_rotation_angle <= -6.28318f)
		m_rotation_angle += -6.28318f;

	m_rotation_matrix = glm::rotate(m_rotation_angle, m_rotation_axis);
}


void Drawable_Object::set_scale(float _scale_x, float _scale_y, float _scale_z)
{
	m_previous_state.scale_matrix = m_scale_matrix;

	m_scale_matrix[0][0] = _scale_x;
	m_scale_matrix[1][1] = _scale_y;
	m_scale_matrix[2][2] = _scale_z;
}

void Drawable_Object::set_overall_scale(float _scale)
{
	m_previous_state.scale_matrix = m_scale_matrix;

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











Colliding_Object::~Colliding_Object()
{
	delete m_physical_model;
}



void Colliding_Object::init_physical_model(const float *_coords, unsigned int _coords_count)
{
	m_can_cause_collision = true;
	delete m_physical_model;
	m_physical_model = nullptr;
	delete m_physical_model_prev_state;
	m_physical_model_prev_state = nullptr;
}

void Colliding_Object::remove_physical_model()
{
	delete m_physical_model;
	m_can_cause_collision = false;
}

void Colliding_Object::init(const char *_object_name)
{
	Drawable_Object::init(_object_name);

	std::pair<const float*, unsigned int> physical_model_data;
	LEti::Resource_Loader::assign(physical_model_data, _object_name, "physical_model_data");
	if((physical_model_data.first))
		init_physical_model(physical_model_data.first, physical_model_data.second);
}



void Colliding_Object::update()
{
	if(m_can_cause_collision)
		m_physical_model->update(m_translation_matrix, m_rotation_matrix, m_scale_matrix);
}

const Physical_Model_Interface* Colliding_Object::get_physical_model_interface() const
{
	return m_physical_model;
}

Physical_Model_Interface* Colliding_Object::get_physical_model_interface()
{
	return m_physical_model;
}

const Physical_Model_Interface* Colliding_Object::get_physical_model_interface_prev_state() const
{
	return m_physical_model_prev_state;
}

Physical_Model_Interface* Colliding_Object::get_physical_model_interface_prev_state()
{
	return m_physical_model_prev_state;
}



void Colliding_Object::set_collision_possibility(bool _can_cause_collision)
{
    m_can_cause_collision = _can_cause_collision;
}

bool Colliding_Object::get_collision_possibility() const
{
    return m_can_cause_collision;
}


void Colliding_Object::set_is_dynamic(bool _is_dynamic)
{
	ASSERT(!m_physical_model || !m_physical_model_prev_state);
	m_physical_model->set_is_dynamic(_is_dynamic);
	m_physical_model_prev_state->set_is_dynamic(_is_dynamic);
}

bool Colliding_Object::is_dynamic() const
{
	ASSERT(!m_physical_model || !m_physical_model_prev_state);
	return m_physical_model->is_dynamic();
}


LEti::Physical_Model_Interface::Intersection_Data Colliding_Object::is_colliding_with_other(const Colliding_Object& _other) const
{
    ASSERT(m_can_cause_collision && !m_physical_model);
    ASSERT(_other.m_can_cause_collision && !_other.m_physical_model);
	if(!_other.m_can_cause_collision || !m_can_cause_collision)
		return LEti::Physical_Model_Interface::Intersection_Data(LEti::Physical_Model_Interface::Intersection_Data::Intersection_Type::none);
    return m_physical_model->is_intersecting_with_another_model(*_other.m_physical_model);
}
















Object_2D::Object_2D() : Colliding_Object()
{

}

Object_2D::~Object_2D()
{

}


void Object_2D::init_physical_model(const float *_coords, unsigned int _coords_count)
{
	Colliding_Object::init_physical_model(_coords, _coords_count);
    m_physical_model = new Physical_Model_2D(_coords, _coords_count);
	m_physical_model_prev_state = new Physical_Model_2D(_coords, _coords_count);
}


void Object_2D::init(const char* _object_name)
{
	Colliding_Object::init(_object_name);
}



void Object_2D::draw() const
{
	LEti::Camera::use_2d();
    glDisable(GL_DEPTH_TEST);

	Colliding_Object::draw();
}

void Object_2D::update()
{
	if(m_can_cause_collision)
	{
		get_physical_model_prev_state()->copy_real_coordinates(*get_physical_model());
		if(is_dynamic())
		{
			const Physical_Model_2D::Rectangular_Border& prev_rb = get_physical_model_prev_state()->curr_rect_border(),
					curr_rb = get_physical_model()->curr_rect_border();
			m_dynamic_rb.left = prev_rb.left < curr_rb.left ? prev_rb.left : curr_rb.left;
			m_dynamic_rb.right = prev_rb.right > curr_rb.right ? prev_rb.right : curr_rb.right;
			m_dynamic_rb.top = prev_rb.top > curr_rb.top ? prev_rb.top : curr_rb.top;
			m_dynamic_rb.bottom = prev_rb.bottom < curr_rb.bottom ? prev_rb.bottom : curr_rb.bottom;
		}
	}
	Colliding_Object::update();
}



Physical_Model_2D* Object_2D::get_physical_model()
{
	return (Physical_Model_2D*)get_physical_model_interface();
}

Physical_Model_2D* Object_2D::get_physical_model_prev_state()
{
	return (Physical_Model_2D*)get_physical_model_interface_prev_state();
}


const Physical_Model_2D* Object_2D::get_physical_model() const
{
	return (const Physical_Model_2D*)get_physical_model_interface();
}

const Physical_Model_2D* Object_2D::get_physical_model_prev_state() const
{
	return (const Physical_Model_2D*)get_physical_model_interface_prev_state();
}

const Physical_Model_2D::Rectangular_Border& Object_2D::get_dynamic_rb() const
{
	return m_dynamic_rb;
}



LEti::Physical_Model_Interface::Intersection_Data Object_2D::get_precise_time_ratio_of_collision(unsigned int _level, const Object_2D& _other, bool _collision_detected, float _min_ratio, float _max_ratio) const
{
	ASSERT(!m_can_cause_collision || !_other.m_can_cause_collision);
	ASSERT(!is_dynamic() && !_other.is_dynamic());

	float time_mid_point = (_max_ratio + _min_ratio) / 2.0f;

	Physical_Model_Interface::Intersection_Data result;

	if(_level < m_precision_level)
	{
		result = get_precise_time_ratio_of_collision(_level + 1, _other, false, _min_ratio, time_mid_point);
		if(result)
			return result;

		result = get_precise_time_ratio_of_collision(_level + 1, _other, false, time_mid_point, _max_ratio);
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
	return result;
}



LEti::Physical_Model_Interface::Intersection_Data Object_2D::is_colliding_with_other(const Colliding_Object& _other) const
{
	ASSERT(m_can_cause_collision && !m_physical_model);
	ASSERT(_other.get_collision_possibility() && !_other.get_physical_model_interface());
	if(!_other.is_dynamic() && !is_dynamic())
	{
		if(!_other.get_collision_possibility() || !get_collision_possibility())
			return LEti::Physical_Model_Interface::Intersection_Data(LEti::Physical_Model_Interface::Intersection_Data::Intersection_Type::none);
		return m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model_interface());
	}

	//	TODO: remove this - only for testing purpose
	bool really_colliding = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model_interface());
	//

	const Physical_Model_2D::Rectangular_Border this_rb_prev = get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border this_rb_curr = get_physical_model()->curr_rect_border();

	const Physical_Model_2D::Rectangular_Border other_rb_prev = ((const Object_2D&)_other).get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border other_rb_curr = ((const Object_2D&)_other).get_physical_model()->curr_rect_border();

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
		Utility::get_distance(this_segments[0].first, this_segments[0].second),
		Utility::get_distance(this_segments[1].first, this_segments[1].second),
		Utility::get_distance(this_segments[2].first, this_segments[2].second),
		Utility::get_distance(this_segments[3].first, this_segments[3].second)
	};
	bool this_frame_is_static = true;
	for(unsigned int i=0; i<4; ++i)
	{
		if(!Utility::floats_are_equal(this_segments_lengths[i], 0.0f))
		{
			this_frame_is_static = false;
			break;
		}
	}

	float other_segments_lengths[4] = {
		Utility::get_distance(other_segments[0].first, other_segments[0].second),
		Utility::get_distance(other_segments[1].first, other_segments[1].second),
		Utility::get_distance(other_segments[2].first, other_segments[2].second),
		Utility::get_distance(other_segments[3].first, other_segments[3].second)
	};
	bool other_frame_is_static = true;
	for(unsigned int i=0; i<4; ++i)
	{
		if(!Utility::floats_are_equal(other_segments_lengths[i], 0.0f))
		{
			other_frame_is_static = false;
			break;
		}
	}


	//	case in which both objects are moving and their paths intersects
	float min_ratio = 1.1f, max_ratio = -0.1f;
	for(unsigned int i=0; i<4; ++i)
	{
		for(unsigned int j=0; j<4; ++j)
		{
			LEti::Physical_Model_Interface::Intersection_Data id =
					Physical_Model_2D::segments_intersect(this_segments[i].first, this_segments[i].second,
					other_segments[j].first, other_segments[j].second);
			if(id)
			{
				if(!Utility::floats_are_equal(this_segments_lengths[i], 0.0f))
				{
					float this_ratio = Utility::get_distance(this_segments[i].first, id.closest_intersection_point) / this_segments_lengths[i];
					if(min_ratio > this_ratio) min_ratio = this_ratio;
					if(max_ratio < this_ratio) max_ratio = this_ratio;
				}
				if(!Utility::floats_are_equal(other_segments_lengths[i], 0.0f))
				{
					float other_ratio = Utility::get_distance(other_segments[j].first, id.closest_intersection_point) / other_segments_lengths[j];
					if(min_ratio > other_ratio) min_ratio = other_ratio;
					if(max_ratio < other_ratio) max_ratio = other_ratio;
				}
			}
		}
	}
	if(max_ratio > 0.0f && max_ratio <= 1.0f && min_ratio < 1.0f && min_ratio >= 0.0f)
	{
		if(Utility::floats_are_equal(min_ratio, max_ratio))
		{
			std::cout << min_ratio << ' ' << max_ratio << "\n";
			Physical_Model_2D::Intersection_Data result = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model_interface());
			result.time_of_intersection_ratio = min_ratio;
			return result;
		}

	}
//	std::cout << min_ratio << ' ' << max_ratio << "\n";

	//this lambda is wrong
	auto is_in_bounds_of_segment_by_x = [](const std::pair<glm::vec3, glm::vec3>& _segment, const glm::vec3& _point)->bool{
		bool is_first_on_the_left = _segment.first.x < _segment.second.x;
		return is_first_on_the_left ? _segment.first.x <= _point.x && _segment.second.x >= _point.x
									: _segment.second.x <= _point.x && _segment.first.x >= _point.x;
	};

	//	case in which both objects moves, so they may intersect, but their frames' angles' paths does not intersect
	if(!this_frame_is_static && !other_frame_is_static /*&& really_colliding*/)
	{
		for(unsigned int i=0; i<4; ++i)
		{
			std::pair<glm::vec3, glm::vec3>& current_this_segment = this_segments[i];
			Physical_Model_2D::Equasion_Data this_equasion = Physical_Model_2D::get_equasion(current_this_segment.first, current_this_segment.second);

			bool above_found = false, underneath_found = false;
			float temp_min_ratio = 1.1f, temp_max_ratio = -0.1f;

			for(unsigned int j=0; j<4; ++j)
			{
				//check current this state
				std::pair<glm::vec3, glm::vec3>& current_other_segment = other_segments[j];

				//check other previous state
				if(is_in_bounds_of_segment_by_x(current_this_segment, current_other_segment.first))
				{
					float this_y = this_equasion.solve_by_x(current_other_segment.first.x);
					if(this_y <= current_other_segment.first.y) above_found = true;
					if(this_y >= current_other_segment.first.y) underneath_found = true;

					float other_ratio = 0.0f;
					float distance_between_start_and_projection = Utility::get_distance({current_other_segment.first.x, this_y, 0.0f}, current_this_segment.first);
					float this_ratio = distance_between_start_and_projection / this_segments_lengths[i];
					if(this_ratio <= 1.0f && this_ratio >= 0.0f)
					{
						if(temp_min_ratio > other_ratio) temp_min_ratio = other_ratio;
						if(temp_max_ratio < other_ratio) temp_max_ratio = other_ratio;
						if(temp_min_ratio > this_ratio) temp_min_ratio = this_ratio;
						if(temp_max_ratio < this_ratio) temp_max_ratio = this_ratio;
					}
				}
//				else
				{
//					std::cout << "previous state frame does not intersect\n";
				}

				//check other current state
				if(is_in_bounds_of_segment_by_x(current_this_segment, current_other_segment.second))
				{
					float this_y = this_equasion.solve_by_x(current_other_segment.second.x);
					if(this_y <= current_other_segment.second.y) above_found = true;
					if(this_y >= current_other_segment.second.y) underneath_found = true;

					float other_ratio = 1.0f;
					float distance_between_start_and_projection = Utility::get_distance({current_other_segment.first.x, this_y, 0.0f}, current_this_segment.first);
					float this_ratio = distance_between_start_and_projection / this_segments_lengths[i];
					if(this_ratio <= 1.0f && this_ratio >= 0.0f)
					{
						if(temp_min_ratio > other_ratio) temp_min_ratio = other_ratio;
						if(temp_max_ratio < other_ratio) temp_max_ratio = other_ratio;
						if(temp_min_ratio > this_ratio) temp_min_ratio = this_ratio;
						if(temp_max_ratio < this_ratio) temp_max_ratio = this_ratio;
					}
				}
//				else
				{
//					std::cout << "current state frame does not intersect\n";
				}
			}

			if(above_found && underneath_found)
			{
				min_ratio = temp_min_ratio;
				max_ratio = temp_max_ratio;
				int a =0;
				++a;
			}
		}
	}
	if(max_ratio > 0.0f && max_ratio <= 1.0f && min_ratio < 1.0f && min_ratio >= 0.0f)
	{
		if(Utility::floats_are_equal(min_ratio, max_ratio))
		{
			std::cout << min_ratio << ' ' << max_ratio << "\n";
			Physical_Model_2D::Intersection_Data result = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model_interface());
			result.time_of_intersection_ratio = min_ratio;
			return result;
		}
		else
		{
			Physical_Model_2D::Intersection_Data result = get_precise_time_ratio_of_collision(0, (const Object_2D&)_other, false, min_ratio, max_ratio);
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
//	bool only_one_frame_is_static = (this_frame_is_static && !other_frame_is_static) || (!this_frame_is_static && other_frame_is_static);
//	if(only_one_frame_is_static)
//	{
//		const Object_2D& static_object = this_frame_is_static ? *this : (const Object_2D&)_other;
//		const Object_2D& dynamic_object = other_frame_is_static ? *this : (const Object_2D&)_other;


//	}

	if(really_colliding && this_segments_lengths[0] > 0.0f && other_segments_lengths[0] > 0.0f)
	{
//		std::cout << "(2 are moving) collision should be here, but it's not detected!\n";
	}
	if(really_colliding && ((Utility::floats_are_equal(this_segments_lengths[0], 0.0f) && other_segments_lengths[0] > 0.0f) || (Utility::floats_are_equal(this_segments_lengths[0], 0.0f) && other_segments_lengths[0] > 0.0f)))
	{
		std::cout << "(1 is moving) collision should be here, but it's not detected!\n";
	}

//	return LEti::Physical_Model_Interface::Intersection_Data(LEti::Physical_Model_Interface::Intersection_Data::Intersection_Type::none);
	return m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model_interface());
}










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
