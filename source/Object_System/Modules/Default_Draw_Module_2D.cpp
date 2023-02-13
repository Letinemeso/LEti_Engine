#include "../../../include/Object_System/Modules/Default_Draw_Module_2D.h"

using namespace LEti;


Default_Draw_Module_2D::Default_Draw_Module_2D() : Draw_Module_Base()
{
	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	m_vertices.vertex_array = &m_vertex_array;
	m_texture.vertex_array = &m_vertex_array;
    m_colors.vertex_array = &m_vertex_array;
}

Default_Draw_Module_2D::~Default_Draw_Module_2D()
{
	glDeleteVertexArrays(1, &m_vertex_array);
}



void Default_Draw_Module_2D::init_vertices(const float *const _coords, unsigned int _coords_count)
{
    glBindVertexArray(m_vertex_array);
    m_vertices.init(_coords, _coords_count);
    m_vertices.setup_buffer(0, 3);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}

void Default_Draw_Module_2D::init_colors(const float *const _colors, unsigned int _colors_count)
{
    glBindVertexArray(m_vertex_array);
    m_colors.init(_colors, _colors_count);
    m_colors.setup_buffer(1, 4);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}

void Default_Draw_Module_2D::init_texture(const Picture* _picture, const float *const tex_coords, unsigned int _tex_coords_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.init(_picture, tex_coords, _tex_coords_count);
    m_texture.setup_buffer(2, 2);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}


void Default_Draw_Module_2D::set_texture(const Picture* _picture)
{
	glBindVertexArray(m_vertex_array);
	m_texture.set_picture(_picture);
}

void Default_Draw_Module_2D::set_texture_coords(const float* _tc, unsigned int _tc_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.set_texture_coords(_tc, _tc_count);
    m_texture.setup_buffer(2, 2);
}
