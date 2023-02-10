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

	sequence = *(std::unsigned_string*)&stub->sequence;

	create_draw_module();
	draw_module()->set_texture(Picture_Manager::get_picture(stub->font_texture));
	text_tex_coords.first = stub->tcoords;
	text_tex_coords.second = stub->tcoords_count;

	L_ASSERT(!(text_tex_coords.second < sequence.size()* tcpc));
	for (unsigned int i = 0; i < sequence.size(); ++i)
	{
		unsigned char current_char = sequence[i];
		L_ASSERT(!(sequence_map.find(current_char) != sequence_map.end()));
		sequence_map.emplace(current_char, &(text_tex_coords.first[tcpc * i]));
	}

	set_pos(stub->position);
	width = stub->width;
	height = stub->height;
}



void Text_Field::set_text(const char* _text)
{
	text = (const unsigned char*)_text;

	if (text.size() == 0) return;

	float* temp_coords = new float[text.size() * tcpc];
	for (unsigned int i = 0; i < text.size(); ++i)
	{
		const float* ptr = sequence_map.at(text[i]);
		L_ASSERT(!(ptr == nullptr));

		for (unsigned int j = 0; j < tcpc; ++j)
			temp_coords[i * tcpc + j] = ptr[j];
	}
    m_draw_module->set_texture_coords(temp_coords, text.size() * tcpc);
	delete[] temp_coords;

	actual_symbol_width = (height * text.size() > width ? width / text.size() : height);
	temp_coords = new float[text.size() * cpc];
	for (unsigned int i = 0; i < text.size(); ++i)
	{
		std::pair<float, float> vertices[4];
		vertices[0].first = actual_symbol_width * (float)i;							vertices[0].second = height;
		vertices[1].first = actual_symbol_width * (float)i;							vertices[1].second = 0.0f;
		vertices[2].first = actual_symbol_width * (float)i + actual_symbol_width;	vertices[2].second = 0.0f;
		vertices[3].first = actual_symbol_width * (float)i + actual_symbol_width;	vertices[3].second = height;

		temp_coords[0 + cpc * i] = vertices[0].first; temp_coords[1 + cpc * i] = vertices[0].second;
		temp_coords[3 + cpc * i] = vertices[1].first; temp_coords[4 + cpc * i] = vertices[1].second;
		temp_coords[6 + cpc * i] = vertices[2].first; temp_coords[7 + cpc * i] = vertices[2].second;
		temp_coords[9 + cpc * i] = vertices[0].first; temp_coords[10+ cpc * i] = vertices[0].second;
		temp_coords[12+ cpc * i] = vertices[2].first; temp_coords[13+ cpc * i] = vertices[2].second;
		temp_coords[15+ cpc * i] = vertices[3].first; temp_coords[16+ cpc * i] = vertices[3].second;
	}
	for (unsigned int i = 2; i < text.size() * cpc; i += 3)
		temp_coords[i] = 0.0f;
    m_draw_module->init_vertices(temp_coords, text.size() * cpc);

    unsigned int colors_count = m_draw_module->vertices().size() / 3 * 4;
    float* colors = new float[colors_count];
    for(unsigned int i=0; i<colors_count; ++i)
        colors[i] = 1.0f;
    m_draw_module->init_colors(colors, colors_count);
    delete[] colors;

	delete[] temp_coords;
}

const std::unsigned_string& Text_Field::get_text() const
{
	return text;
}



void Text_Field::draw() const
{
    if (text.size() == 0)
        return;
    Object_2D::draw();
}
