#include "../../../include/Object_System/Modules/Default_Draw_Module_2D.h"

using namespace TEST;


Default_Draw_Module_2D::Default_Draw_Module_2D() : Draw_Module_Base()
{
	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	m_vertices.vertex_array = &m_vertex_array;
	m_texture.vertex_array = &m_vertex_array;
}

Default_Draw_Module_2D::~Default_Draw_Module_2D()
{
	glDeleteVertexArrays(1, &m_vertex_array);
}



void Default_Draw_Module_2D::init_texture(const char *_picture_name, const float *const tex_coords, unsigned int _tex_coords_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.init(_picture_name, tex_coords, _tex_coords_count);
	m_texture.setup_buffer(1, 2);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}

void Default_Draw_Module_2D::init_vertices(const float *const _coords, unsigned int _coords_count)
{
	glBindVertexArray(m_vertex_array);
	m_vertices.load(_coords, _coords_count);
	m_vertices.setup_buffer(0, 3);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}



void Default_Draw_Module_2D::draw(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 _scale) const
{
	if (!m_visible) return;

	ASSERT(m_vertex_array == 0 || m_vertices.get_vertices_count() == 0 || m_texture.size() == 0);

	LEti::Camera::use_2d();

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;
	LEti::Shader::set_transform_matrix(result_matrix);

	glBindVertexArray(m_vertex_array);
	LEti::Shader::set_texture(m_texture);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.get_vertices_count());

	glBindVertexArray(0);
}



LEti::Texture& Default_Draw_Module_2D::get_texture()
{
	return m_texture;
}

LEti::Vertices& Default_Draw_Module_2D::get_vertices()
{
	return m_vertices;
}
