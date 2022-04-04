#include "../include/Object.h"

using namespace LEti;

Object::Object()
{
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	translation_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	rotation_matrix = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_axis[0] = 0.0f;
	rotation_axis[1] = 1.0f;
	rotation_axis[2] = 0.0f;
	
	scale_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Object::~Object()
{
	glDeleteVertexArrays(1, &vertex_array);
}



void Object::init_texture(const char* _tex_path, const float* const tex_coords, unsigned int _tex_coords_count)
{
	glBindVertexArray(vertex_array);
	texture.init(_tex_path, tex_coords, _tex_coords_count);
	texture.setup_buffer(1, 2);
}

void Object::init_vertices(const float* const _coords, unsigned int _coords_count)
{
	glBindVertexArray(vertex_array);
	vertices.load(_coords, _coords_count);
	vertices.setup_buffer(0, 3);

//    cbuf.allocate_memory()
}

void Object::init(const char* _object_name)
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
	set_pos(scale.first[0], scale.first[1], scale.first[2]);

	auto raxis = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_axis");
	ASSERT(raxis.second != 3);
	set_pos(raxis.first[0], raxis.first[1], raxis.first[2]);

	auto rangle = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_angle");
	ASSERT(rangle.second != 1);
	set_pos(rangle.first[0], rangle.first[1], rangle.first[2]);

	auto i3d = LEti::Resource_Loader::get_data<unsigned int>(_object_name, "is_3d");
	ASSERT(i3d.second != 1 || *i3d.first > 1);
	set_is_3d(*i3d.first);
}



void Object::set_texture(const char* _path)
{
	glBindVertexArray(vertex_array);
	texture.set_picture(_path);
}

void Object::set_texture_coords(const float* _tc, unsigned int _tc_count)
{
	glBindVertexArray(vertex_array);
	texture.set_texture_coords(_tc, _tc_count);
	texture.setup_buffer(1, 2);
}



void Object::draw() const
{
	if (!visible) return;

	ASSERT(vertex_array == 0 || vertices.get_vertices_count() == 0 || texture.size() == 0);

	glm::mat4x4 result_matrix = translation_matrix * rotation_matrix * scale_matrix;
	LEti::Shader::set_transform_matrix(result_matrix);

	LEti::Camera::use(is_3d);

	glBindVertexArray(vertex_array);
	
	LEti::Shader::set_texture(texture);
	glDrawArrays(GL_TRIANGLES, 0, vertices.get_vertices_count());

	glBindVertexArray(0);
}

void Object::update(float _dt)
{
	
}



void Object::set_is_3d(bool _is_3d)
{
	is_3d = _is_3d;
}

void Object::set_visible(bool _visible)
{
	visible = _visible;
}

bool Object::get_visible() const
{
	return visible;
}


void Object::set_pos(float _x, float _y, float _z)
{
	translation_matrix[3][0] = _x;
	translation_matrix[3][1] = _y;
	translation_matrix[3][2] = _z;
}

void Object::move(float _x, float _y, float _z)
{
	translation_matrix[3][0] += _x;
	translation_matrix[3][1] += _y;
	translation_matrix[3][2] += _z;
}


void Object::set_rotation_axis(float _x, float _y, float _z)
{
	rotation_axis[0] = _x;
	rotation_axis[1] = _y;
	rotation_axis[2] = _z;

	rotation_matrix = glm::rotate(rotation_angle, rotation_axis);
}

void Object::set_rotation_angle(float _angle)
{
	rotation_angle = _angle;

	while (rotation_angle >= 6.28318f)
		rotation_angle -= 6.28318f;
	while (rotation_angle <= -6.28318f)
		rotation_angle += -6.28318f;

	rotation_matrix = glm::rotate(rotation_angle, rotation_axis);
}

void Object::set_rotation_data(float _axis_x, float _axis_y, float _axis_z, float _angle)
{
	rotation_axis[0] = _axis_x;
	rotation_axis[1] = _axis_y;
	rotation_axis[2] = _axis_z;
	rotation_angle = _angle;

	rotation_matrix = glm::rotate(rotation_angle, rotation_axis);
}

void Object::rotate(float _angle)
{
	rotation_angle += _angle;

	if (rotation_angle >= 6.28318f)
		rotation_angle -= 6.28318f;
	if (rotation_angle <= -6.28318f)
		rotation_angle += -6.28318f;

	rotation_matrix = glm::rotate(rotation_angle, rotation_axis);
}


void Object::set_scale(float _scale_x, float _scale_y, float _scale_z)
{
	scale_matrix[0][0] = _scale_x;
	scale_matrix[1][1] = _scale_y;
	scale_matrix[2][2] = _scale_z;
}

void Object::set_overall_scale(float _scale)
{
	Object::set_scale(_scale, _scale, _scale);
}
