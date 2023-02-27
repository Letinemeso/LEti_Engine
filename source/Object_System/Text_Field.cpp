#include <include/Object_System/Text_Field.h>

using namespace LEti;


constexpr unsigned int tcpc = 12, cpc = 18, colorpc = 24;		//texture coordinates per character, coordinates per character



INIT_FIELDS(Text_Field_Stub, LV::Variable_Base)

ADD_FIELD(std::string, font_texture)
ADD_FIELD(unsigned int, tcoords_count)
ADD_FIELD(float*, tcoords)
ADD_FIELD(glm::vec3, position)
ADD_FIELD(float, width)
ADD_FIELD(float, height)
ADD_FIELD(std::string, sequence)

FIELDS_END



Text_Field::Text_Field() : Object_2D()
{

}

void Text_Field::init(const LV::Variable_Base &_stub)
{
	const Text_Field_Stub* stub = LV::cast_variable<Text_Field_Stub>(&_stub);
	L_ASSERT(stub);

	remove_draw_module();
	remove_physics_module();

    m_sequence = stub->sequence;

	create_draw_module();

	draw_module()->set_texture(Picture_Manager::get_picture(stub->font_texture));
    m_text_tex_coords.first = stub->tcoords;
    m_text_tex_coords.second = stub->tcoords_count;

    L_ASSERT(!(m_text_tex_coords.second < m_sequence.size()* tcpc));
    for (unsigned int i = 0; i < m_sequence.size(); ++i)
	{
        unsigned char current_char = m_sequence[i];
        L_ASSERT(!(m_sequence_map.find(current_char) != m_sequence_map.end()));
        m_sequence_map.emplace(current_char, &(m_text_tex_coords.first[tcpc * i]));
	}

	set_pos(stub->position);
    m_width = stub->width;
    m_height = stub->height;
}



void Text_Field::set_text(const std::string& _text)
{
    m_text = _text;

    if (m_text.size() == 0)
        return;

    float* temp_coords = new float[m_text.size() * tcpc];
    for (unsigned int i = 0; i < m_text.size(); ++i)
	{
        const float* ptr = m_sequence_map.at(m_text[i]);
		L_ASSERT(!(ptr == nullptr));

		for (unsigned int j = 0; j < tcpc; ++j)
			temp_coords[i * tcpc + j] = ptr[j];
	}
    m_draw_module->set_texture_coords(temp_coords, m_text.size() * tcpc);
	delete[] temp_coords;

    m_actual_symbol_width = (m_height * m_text.size() > m_width ? m_width / m_text.size() : m_height);
    temp_coords = new float[m_text.size() * cpc];

    float h_alignment_stride = 0.0f;
    if(m_horizontal_alignment == Horizontal_Alignment::right)
        h_alignment_stride = m_actual_symbol_width * m_text.size();
    else if(m_horizontal_alignment == Horizontal_Alignment::center)
        h_alignment_stride = m_actual_symbol_width * m_text.size() * 0.5f;

    float v_alignment_stride = 0.0f;
    if(m_vertical_alignment == Vertical_Alignment::top)
        v_alignment_stride = m_height;
    else if(m_vertical_alignment == Vertical_Alignment::center)
        v_alignment_stride = m_height * 0.5f;

    for (unsigned int i = 0; i < m_text.size(); ++i)
	{
		std::pair<float, float> vertices[4];
        vertices[0].first = m_actual_symbol_width * (float)i - h_alignment_stride;							vertices[0].second = m_height - v_alignment_stride;
        vertices[1].first = m_actual_symbol_width * (float)i - h_alignment_stride;							vertices[1].second = 0.0f - v_alignment_stride;
        vertices[2].first = m_actual_symbol_width * (float)i + m_actual_symbol_width - h_alignment_stride;	vertices[2].second = 0.0f - v_alignment_stride;
        vertices[3].first = m_actual_symbol_width * (float)i + m_actual_symbol_width - h_alignment_stride;	vertices[3].second = m_height - v_alignment_stride;

		temp_coords[0 + cpc * i] = vertices[0].first; temp_coords[1 + cpc * i] = vertices[0].second;
		temp_coords[3 + cpc * i] = vertices[1].first; temp_coords[4 + cpc * i] = vertices[1].second;
		temp_coords[6 + cpc * i] = vertices[2].first; temp_coords[7 + cpc * i] = vertices[2].second;
		temp_coords[9 + cpc * i] = vertices[0].first; temp_coords[10+ cpc * i] = vertices[0].second;
		temp_coords[12+ cpc * i] = vertices[2].first; temp_coords[13+ cpc * i] = vertices[2].second;
		temp_coords[15+ cpc * i] = vertices[3].first; temp_coords[16+ cpc * i] = vertices[3].second;
	}
    for (unsigned int i = 2; i < m_text.size() * cpc; i += 3)
		temp_coords[i] = 0.0f;
    m_draw_module->init_vertices(temp_coords, m_text.size() * cpc);
    delete[] temp_coords;

    unsigned int colors_count = m_text.size() * colorpc;
    float* colors = new float[colors_count];
    for(unsigned int i=0; i<colors_count; ++i)
        colors[i] = 1.0f;
    m_draw_module->init_colors(colors, colors_count);
    delete[] colors;
}
