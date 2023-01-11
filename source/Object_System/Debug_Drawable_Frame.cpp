#include <include/Object_System/Debug_Drawable_Frame.h>

using namespace LEti;


void Debug_Line_Draw_Module::draw(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 _scale) const
{
	if (!m_visible) return;

	ASSERT(m_vertex_array == 0 || m_vertices.get_vertices_count() == 0 || m_texture.size() == 0);

	LEti::Camera::use_2d();

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;
	LEti::Shader::set_transform_matrix(result_matrix);

	glBindVertexArray(m_vertex_array);
	LEti::Shader::set_texture(m_texture);
	glDrawArrays(GL_LINES, 0, m_vertices.get_vertices_count());

	glBindVertexArray(0);
}



Debug_Drawable_Frame& Debug_Drawable_Frame::set_point(unsigned int _index, const glm::vec3 &_point)
{
	if(_index >= m_points.size())
		m_points.resize(_index + 1);
	m_points[_index] = _point;

	m_changes_were_made = true;
	return *this;
}

Debug_Drawable_Frame& Debug_Drawable_Frame::set_sequence_element(unsigned int _index, unsigned int _point_index)
{
	if(_index >= m_sequence.size())
		m_sequence.resize(_index + 1);
	m_sequence[_index] = _point_index;

	m_changes_were_made = true;
	return *this;
}

Debug_Drawable_Frame& Debug_Drawable_Frame::clear_points()
{
	m_points.clear();
	return *this;
}

Debug_Drawable_Frame& Debug_Drawable_Frame::clear_sequence()
{
	m_sequence.clear();
	return *this;
}



void Debug_Drawable_Frame::create_draw_module()
{
	remove_draw_module();
	m_draw_module = new Debug_Line_Draw_Module;
}



void Debug_Drawable_Frame::update(float _ratio)
{
	Object_2D::update(_ratio);

	if(!m_changes_were_made) return;

	unsigned int total_coords_count = m_sequence.size() * 6;
	unsigned int total_tex_coords_count = m_sequence.size() * 4;
	draw_module()->get_vertices().free_memory();
	draw_module()->get_vertices().allocate_memory(total_coords_count);
	draw_module()->get_vertices().setup_buffer(0, 3);

	for(unsigned int se=0; se<m_sequence.size() - 1; ++se)
	{
		for(unsigned int i=0; i<3; ++i)
		{
			draw_module()->get_vertices()[se * 6 + i] = m_points[m_sequence[se]][i];
			draw_module()->get_vertices()[se * 6 + (i+3)] = m_points[m_sequence[se + 1]][i];
		}
	}
	for(unsigned int i=0; i<3; ++i)
	{
		draw_module()->get_vertices()[(m_sequence.size()-1) * 6 + i] = m_points[m_sequence[m_sequence.size()-1]][i];
		draw_module()->get_vertices()[(m_sequence.size()-1) * 6 + (i+3)] = m_points[m_sequence[0]][i];
	}

	draw_module()->get_texture().free_memory();
	draw_module()->get_texture().allocate_memory(total_tex_coords_count);
	draw_module()->get_texture().setup_buffer(1, 2);

	for(unsigned int i=0; i<total_tex_coords_count; ++i)
		draw_module()->get_texture()[i] = 0.0f;

	m_changes_were_made = false;
}
