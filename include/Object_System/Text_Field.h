#ifndef __TEXT_FIELD
#define __TEXT_FIELD

//#include "../include/Object.h"
#include <Object_System/Object_2D.h>

#include <string>

namespace LEti
{

	class Text_Field_Stub : public LV::Variable_Base
	{
	public:
		DECLARE_VARIABLE;

	public:
		std::string font_texture;

		unsigned int tcoords_count = 0;
		float* tcoords = nullptr;

		glm::vec3 position;
		float width = 0, height = 0;

		std::string sequence;

	};

	class Text_Field : public Object_2D
	{
	private:
        std::string sequence;
		std::pair<const float*, unsigned int> text_tex_coords;

		std::map<unsigned char, const float*> sequence_map;

	private:
		float width = 0.0f, height = 0.0f, actual_symbol_width = 0.0f;
        std::string text;

	public:
		Text_Field();

	public:
		void init(const LV::Variable_Base& _stub) override;

	public:
        void set_text(const std::string& _text);
        const std::string& get_text() const;

	};


}

#endif
