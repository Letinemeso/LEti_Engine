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


bool Drawable_Object::moved_since_last_frame() const
{
	for(unsigned int x=0; x<4; ++x)
		for(unsigned int y=0; y<4; ++y)
			if(!Math::floats_are_equal(m_translation_matrix[x][y], m_previous_state.translation_matrix[x][y]))
				return true;
	for(unsigned int x=0; x<4; ++x)
		for(unsigned int y=0; y<4; ++y)
			if(!Math::floats_are_equal(m_rotation_matrix[x][y], m_previous_state.rotation_matrix[x][y]))
				return true;
	for(unsigned int x=0; x<4; ++x)
		for(unsigned int y=0; y<4; ++y)
			if(!Math::floats_are_equal(m_scale_matrix[x][y], m_previous_state.scale_matrix[x][y]))
				return true;
	return false;
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



void Object_2D::get_object_trajectory(const Object_2D &_obj, std::pair<glm::vec3, glm::vec3> *_result_segments, float *_result_segments_lengths)
{
	ASSERT(!_result_segments || !_result_segments_lengths);

	const Physical_Model_2D::Rectangular_Border& rb_prev = _obj.get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& rb_curr = _obj.get_physical_model()->curr_rect_border();

	_result_segments[0] = {{rb_prev.left, rb_prev.top, 0.0f}, {rb_curr.left, rb_curr.top, 0.0f}};
	_result_segments[1] = {{rb_prev.right, rb_prev.top, 0.0f}, {rb_curr.right, rb_curr.top, 0.0f}};
	_result_segments[2] = {{rb_prev.left, rb_prev.bottom, 0.0f}, {rb_curr.left, rb_curr.bottom, 0.0f}};
	_result_segments[3] = {{rb_prev.right, rb_prev.bottom, 0.0f}, {rb_curr.right, rb_curr.bottom, 0.0f}};

	for(unsigned int i=0; i<4; ++i)
		_result_segments_lengths[i] = Math::get_distance(_result_segments[i].first, _result_segments[i].second);
}

void Object_2D::get_rect_border_segments(const Physical_Model_2D::Rectangular_Border& _rb, std::pair<glm::vec3, glm::vec3> *_result_segments)
{
	ASSERT(!_result_segments);

	_result_segments[0] = {{_rb.left, _rb.top, 0.0f}, {_rb.left, _rb.bottom, 0.0f}};
	_result_segments[1] = {{_rb.left, _rb.bottom, 0.0f}, {_rb.right, _rb.bottom, 0.0f}};
	_result_segments[2] = {{_rb.right, _rb.bottom, 0.0f}, {_rb.right, _rb.top, 0.0f}};
	_result_segments[3] = {{_rb.right, _rb.top, 0.0f}, {_rb.left, _rb.top, 0.0f}};
}


Geometry::Intersection_Data Object_2D::get_precise_time_ratio_of_collision(const Object_2D& _first, const Object_2D& _second, float _min_ratio, float _max_ratio, unsigned int _precision)
{
	ASSERT(!_first.m_can_cause_collision || !_second.m_can_cause_collision);
	ASSERT(!_first.is_dynamic() && !_second.is_dynamic());
	ASSERT(_min_ratio < 0.0f || _max_ratio < 0.0f || _min_ratio > 1.0f || _max_ratio > 1.0f);

	float diff = _max_ratio - _min_ratio;
	float step_diff = diff / (float)_precision;

	float curr_time_point = _min_ratio;
	while(curr_time_point <= _max_ratio)
	{
		Physical_Model_2D::Imprint this_impr = *_first.m_physical_model_prev_state;
		this_impr.update(_first.get_translation_matrix_for_time_ratio(curr_time_point), _first.get_rotation_matrix_for_time_ratio(curr_time_point), _first.get_scale_matrix_for_time_ratio(curr_time_point));
		Physical_Model_2D::Imprint other_impr = *_second.m_physical_model_prev_state;
		other_impr.update(_second.get_translation_matrix_for_time_ratio(curr_time_point), _second.get_rotation_matrix_for_time_ratio(curr_time_point), _second.get_scale_matrix_for_time_ratio(curr_time_point));
		Geometry::Intersection_Data id = this_impr.imprints_intersect(other_impr);
		if(id) { id.time_of_intersection_ratio = Math::floats_are_equal(curr_time_point, _min_ratio) ? curr_time_point : curr_time_point - step_diff; return id; }

		curr_time_point += step_diff;
	}

	return Geometry::Intersection_Data();
}

Geometry::Intersection_Data Object_2D::collision__moving_vs_moving(const Object_2D &_moving_1, const Object_2D &_moving_2)
{
	float min_intersection_ratio = 1.1f, max_intersection_ratio = -0.1f;
	auto rewrite_min_max_ratio = [&min_intersection_ratio, &max_intersection_ratio](float _new)->void
	{
		if(min_intersection_ratio > _new)
			min_intersection_ratio = _new;
		if(max_intersection_ratio < _new)
			max_intersection_ratio = _new;
	};

	Physical_Model_2D::Rectangular_Border shared_space = _moving_1.get_dynamic_rb() && _moving_2.get_dynamic_rb();

	bool first_on_left = _moving_1.m_dynamic_rb.left < _moving_2.m_dynamic_rb.left;
	const Physical_Model_2D::Rectangular_Border& on_left = first_on_left ? _moving_1.m_dynamic_rb : _moving_2.m_dynamic_rb;
	const Physical_Model_2D::Rectangular_Border& on_right = first_on_left ? _moving_2.m_dynamic_rb : _moving_1.m_dynamic_rb;
	bool first_on_bottom = _moving_1.m_dynamic_rb.bottom < _moving_2.m_dynamic_rb.bottom;
	const Physical_Model_2D::Rectangular_Border& on_bottom = first_on_bottom ? _moving_1.m_dynamic_rb : _moving_2.m_dynamic_rb;
	const Physical_Model_2D::Rectangular_Border& on_top = first_on_bottom ? _moving_2.m_dynamic_rb : _moving_1.m_dynamic_rb;

	float on_left_width = on_left.right - on_left.left;
	float on_right_width = on_right.right - on_right.left;
	float on_top_height = on_top.top - on_top.bottom;
	float on_bottom_height = on_bottom.top - on_bottom.bottom;

	float tr_x_1 = (shared_space.left - on_left.left) / on_left_width;
	float tr_x_2 = (on_right.right - shared_space.right) / on_right_width;
	float tr_x_3 = (on_left.right - shared_space.left) / on_left_width;
	float tr_x_4 = (shared_space.right - on_right.left) / on_right_width;

	float tr_y_1 = (shared_space.bottom - on_bottom.bottom) / on_bottom_height;
	float tr_y_2 = (on_top.top - shared_space.top) / on_top_height;
	float tr_y_3 = (on_bottom.top - shared_space.bottom) / on_bottom_height;
	float tr_y_4 = (shared_space.top - on_top.bottom) / on_top_height;

	bool fully_inside_x = (shared_space.left <= on_left.left && shared_space.right >= on_left.right)
			|| (shared_space.left <= on_right.left && shared_space.right >= on_right.right);
	bool fully_inside_y = (shared_space.bottom <= on_left.bottom && shared_space.top >= on_left.top)
			|| (shared_space.bottom <= on_right.bottom && shared_space.top >= on_right.top);

	if(fully_inside_x && fully_inside_y)
	{
		min_intersection_ratio = 0.0f;
		max_intersection_ratio = 1.0f;
	}
	else if(fully_inside_x)
	{
		rewrite_min_max_ratio(tr_y_1);
		rewrite_min_max_ratio(tr_y_2);
		rewrite_min_max_ratio(tr_y_3);
		rewrite_min_max_ratio(tr_y_4);
	}
	else if(fully_inside_y)
	{
		rewrite_min_max_ratio(tr_x_1);
		rewrite_min_max_ratio(tr_x_2);
		rewrite_min_max_ratio(tr_x_3);
		rewrite_min_max_ratio(tr_x_4);
	}
	else
	{
		rewrite_min_max_ratio(tr_x_1);
		rewrite_min_max_ratio(tr_x_2);
		rewrite_min_max_ratio(tr_x_3);
		rewrite_min_max_ratio(tr_x_4);
		rewrite_min_max_ratio(tr_y_1);
		rewrite_min_max_ratio(tr_y_2);
		rewrite_min_max_ratio(tr_y_3);
		rewrite_min_max_ratio(tr_y_4);
	}

	if(min_intersection_ratio <= 1.0f && min_intersection_ratio >= 0.0f && max_intersection_ratio <= 1.0f && max_intersection_ratio >= 0.0f)
	{
		if(Math::floats_are_equal(min_intersection_ratio, max_intersection_ratio))
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_moving_1, _moving_2, min_intersection_ratio, 1.0f, 10);
			return result;
		}
		else
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_moving_1, _moving_2, min_intersection_ratio, max_intersection_ratio, 10);
			return result;
		}
	}

	return Geometry::Intersection_Data();
}

Geometry::Intersection_Data Object_2D::collision__moving_vs_static(const Object_2D &_moving, const Object_2D &_static)
{
	float min_intersection_ratio = 1.1f, max_intersection_ratio = -0.1f;
	auto rewrite_min_max_ratio = [&min_intersection_ratio, &max_intersection_ratio](float _new)->void
	{
		if(min_intersection_ratio > _new)
			min_intersection_ratio = _new;
		if(max_intersection_ratio < _new)
			max_intersection_ratio = _new;
	};

	Physical_Model_2D::Rectangular_Border shared_space = _moving.get_dynamic_rb() && _static.get_physical_model()->curr_rect_border();

	bool first_on_left = _moving.m_dynamic_rb.left < _static.get_physical_model()->curr_rect_border().left;
	const Physical_Model_2D::Rectangular_Border& on_left = first_on_left ? _moving.get_dynamic_rb() : _static.get_physical_model()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& on_right = first_on_left ? _static.get_physical_model()->curr_rect_border() : _moving.get_dynamic_rb();
	bool first_on_bottom = _moving.m_dynamic_rb.bottom < _static.get_physical_model()->curr_rect_border().bottom;
	const Physical_Model_2D::Rectangular_Border& on_bottom = first_on_bottom ? _moving.get_dynamic_rb() : _static.get_physical_model()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& on_top = first_on_bottom ? _static.get_physical_model()->curr_rect_border() : _moving.get_dynamic_rb();

	float on_left_width = on_left.right - on_left.left;
	float on_right_width = on_right.right - on_right.left;
	float on_top_height = on_top.top - on_top.bottom;
	float on_bottom_height = on_bottom.top - on_bottom.bottom;

	float tr_x_1 = (shared_space.left - on_left.left) / on_left_width;
	float tr_x_2 = (on_right.right - shared_space.right) / on_right_width;
	float tr_x_3 = (on_left.right - shared_space.left) / on_left_width;
	float tr_x_4 = (shared_space.right - on_right.left) / on_right_width;

	float tr_y_1 = (shared_space.bottom - on_bottom.bottom) / on_bottom_height;
	float tr_y_2 = (on_top.top - shared_space.top) / on_top_height;
	float tr_y_3 = (on_bottom.top - shared_space.bottom) / on_bottom_height;
	float tr_y_4 = (shared_space.top - on_top.bottom) / on_top_height;

	bool fully_inside_x = (shared_space.left <= on_left.left && shared_space.right >= on_left.right)
			|| (shared_space.left <= on_right.left && shared_space.right >= on_right.right);
	bool fully_inside_y = (shared_space.bottom <= on_left.bottom && shared_space.top >= on_left.top)
			|| (shared_space.bottom <= on_right.bottom && shared_space.top >= on_right.top);

	if(fully_inside_x && fully_inside_y)
	{
		min_intersection_ratio = 0.0f;
		max_intersection_ratio = 1.0f;
	}
	else if(fully_inside_x)
	{
		rewrite_min_max_ratio(tr_y_1);
		rewrite_min_max_ratio(tr_y_2);
		rewrite_min_max_ratio(tr_y_3);
		rewrite_min_max_ratio(tr_y_4);
	}
	else if(fully_inside_y)
	{
		rewrite_min_max_ratio(tr_x_1);
		rewrite_min_max_ratio(tr_x_2);
		rewrite_min_max_ratio(tr_x_3);
		rewrite_min_max_ratio(tr_x_4);
	}
	else
	{
		rewrite_min_max_ratio(tr_x_1);
		rewrite_min_max_ratio(tr_x_2);
		rewrite_min_max_ratio(tr_x_3);
		rewrite_min_max_ratio(tr_x_4);
		rewrite_min_max_ratio(tr_y_1);
		rewrite_min_max_ratio(tr_y_2);
		rewrite_min_max_ratio(tr_y_3);
		rewrite_min_max_ratio(tr_y_4);
	}

	if(min_intersection_ratio <= 1.0f && min_intersection_ratio >= 0.0f && max_intersection_ratio <= 1.0f && max_intersection_ratio >= 0.0f)
	{
		if(Math::floats_are_equal(min_intersection_ratio, max_intersection_ratio))
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_moving, _static, min_intersection_ratio, 1.0f, 10);
			return result;
		}
		else
		{
			Geometry::Intersection_Data result = get_precise_time_ratio_of_collision(_moving, _static, min_intersection_ratio, max_intersection_ratio, 10);
			return result;
		}
	}

	return Geometry::Intersection_Data();
}



Geometry::Intersection_Data Object_2D::is_colliding_with_other(const Object_2D& _other) const
{
	ASSERT(m_can_cause_collision && !m_physical_model);
	ASSERT(_other.m_can_cause_collision && !_other.m_physical_model);

	if(!_other.m_can_cause_collision || !m_can_cause_collision)
		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);

	//	TODO: remove this - only for testing purpose
	auto really_colliding = m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
	if(!really_colliding) return Geometry::Intersection_Data();
	//

	if(!_other.is_dynamic() && !is_dynamic())
	{
		return m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
	}
	else if(is_dynamic() ^ _other.is_dynamic())
	{
		const Object_2D& dynamic_object = is_dynamic() ? *this : _other;
		const Object_2D& static_object = is_dynamic() ? _other : *this;
//		Physical_Model_2D::Rectangular_Border check = dynamic_object.m_dynamic_rb && static_object.get_physical_model()->curr_rect_border();
//		if(check == Physical_Model_2D::Rectangular_Border())
//			return Geometry::Intersection_Data();
		if(dynamic_object.moved_since_last_frame())
		{
			Geometry::Intersection_Data cd = collision__moving_vs_static(dynamic_object, static_object);
			if(cd) return cd;
		}
		return m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
	}
	else if(is_dynamic() && _other.is_dynamic())
	{
//		Physical_Model_2D::Rectangular_Border check = m_dynamic_rb && _other.m_dynamic_rb;
//		if(check == Physical_Model_2D::Rectangular_Border())
//			return Geometry::Intersection_Data();

		if(moved_since_last_frame() && _other.moved_since_last_frame())
		{
			Geometry::Intersection_Data cd = collision__moving_vs_moving(*this, _other);
			if(cd) return cd;
		}
		else if(moved_since_last_frame() ^ _other.moved_since_last_frame())
		{
			const Object_2D& static_object = moved_since_last_frame() ? *this : _other;
			const Object_2D& dynamic_object = moved_since_last_frame() ? _other : *this;
			Geometry::Intersection_Data cd = collision__moving_vs_static(dynamic_object, static_object);
			if(cd) return cd;
		}
		else
		{
			return m_physical_model->is_intersecting_with_another_model(*_other.get_physical_model());
		}
	}
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
