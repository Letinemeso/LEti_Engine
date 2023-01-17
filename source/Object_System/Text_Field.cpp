#include <include/Object_System/Text_Field.h>

using namespace LEti;


constexpr unsigned int tcpc = 12, cpc = 18;		//texture coordinates per character, coordinates per character


Text_Field::Text_Field() : Object_2D()
{
//	set_is_3d(false);
}

void Text_Field::init(const char* _object_name)
{
	create_draw_module();

	draw_module()->set_texture(LEti::Resource_Loader::get_data<std::string>(_object_name, "font").first->c_str());

	sequence = LEti::Resource_Loader::get_data<std::unsigned_string>(_object_name, "sequence").first;
	text_tex_coords = LEti::Resource_Loader::get_data<float>(_object_name, "texture_coordinates");
	L_ASSERT(!(text_tex_coords.second < sequence->size()* tcpc));
	for (unsigned int i = 0; i < sequence->size(); ++i)
	{
		unsigned char current_char = (*sequence)[i];
		L_ASSERT(!(sequence_map.find(current_char) != sequence_map.end()));
		sequence_map.emplace(current_char, &(text_tex_coords.first[tcpc * i]));
	}

	auto position = LEti::Resource_Loader::get_data<float>(_object_name, "position");
	L_ASSERT(!(position.second != 2));
	set_pos({position.first[0], position.first[1], 0.0f});

	auto size = LEti::Resource_Loader::get_data<float>(_object_name, "size");
	L_ASSERT(!(size.second != 2));
	width = size.first[0]; height = size.first[1];
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
	draw_module()->set_texture_coords(temp_coords, text.size() * tcpc);
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
	draw_module()->init_vertices(temp_coords, text.size() * cpc);
	delete[] temp_coords;
}

const std::unsigned_string& Text_Field::get_text() const
{
	return text;
}



void Text_Field::draw() const
{
	if (text.size() == 0) return;
    Object_2D::draw();
}
