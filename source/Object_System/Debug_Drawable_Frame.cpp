#include <include/Object_System/Debug_Drawable_Frame.h>

using namespace LEti;



INIT_FIELDS(LEti::Debug_Line_Draw_Module, LR::Default_Draw_Module_2D)
FIELDS_END

INIT_FIELDS(LEti::Debug_Line_Draw_Module_Stub, LR::Default_Draw_Module_2D_Stub)
FIELDS_END

INIT_FIELDS(LEti::Debug_Drawable_Frame, LEti::Object_2D)
FIELDS_END

INIT_FIELDS(LEti::Debug_Drawable_Frame_Stub, LEti::Object_2D_Stub)
FIELDS_END


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



void Debug_Drawable_Frame::update(float _ratio)
{
	Object_2D::update(_ratio);

	if(!m_changes_were_made) return;

	unsigned int total_coords_count = m_sequence.size() * 6;
	unsigned int total_tex_coords_count = m_sequence.size() * 4;
    draw_module()->vertices().free_memory();
    draw_module()->vertices().allocate_memory(total_coords_count);
    draw_module()->vertices().setup_buffer(0, 3);

	for(unsigned int se=0; se<m_sequence.size() - 1; ++se)
	{
		for(unsigned int i=0; i<3; ++i)
		{
            draw_module()->vertices()[se * 6 + i] = m_points[m_sequence[se]][i];
            draw_module()->vertices()[se * 6 + (i+3)] = m_points[m_sequence[se + 1]][i];
        }
	}
	for(unsigned int i=0; i<3; ++i)
	{
        draw_module()->vertices()[(m_sequence.size()-1) * 6 + i] = m_points[m_sequence[m_sequence.size()-1]][i];
        draw_module()->vertices()[(m_sequence.size()-1) * 6 + (i+3)] = m_points[m_sequence[0]][i];
	}

    draw_module()->colors().resize(draw_module()->vertices().size() / 3 * 4);
    draw_module()->colors().setup_buffer(1, 4);
    for(unsigned int i=0; i<draw_module()->vertices().size() / 3 * 4; ++i)
        draw_module()->colors()[i] = 1.0f;

    draw_module()->texture().free_memory();
    draw_module()->texture().allocate_memory(total_tex_coords_count);
    draw_module()->texture().setup_buffer(2, 2);

	for(unsigned int i=0; i<total_tex_coords_count; ++i)
        draw_module()->texture()[i] = 0.0f;

	m_changes_were_made = false;
}
