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
	if(m_can_cause_collision)
	{
		if(is_dynamic())
		{
//			get_physical_model_prev_state()->update_to_current_model_state();
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
	Drawable_Object::update();
}

void Object_2D::update_previous_state()
{
	Drawable_Object::update_previous_state();
	get_physical_model_prev_state()->update_to_current_model_state();
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
