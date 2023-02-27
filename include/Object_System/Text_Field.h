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
    public:
        enum class Horizontal_Alignment
        {
            left,
            right,
            center
        };
        enum class Vertical_Alignment
        {
            top,
            bottom,
            center
        };

	private:
        std::string m_sequence;
        std::pair<const float*, unsigned int> m_text_tex_coords;

        std::map<unsigned char, const float*> m_sequence_map;

        Horizontal_Alignment m_horizontal_alignment = Horizontal_Alignment::left;
        Vertical_Alignment m_vertical_alignment = Vertical_Alignment::bottom;

	private:
        float m_width = 0.0f, m_height = 0.0f, m_actual_symbol_width = 0.0f;
        std::string m_text;

	public:
		Text_Field();

	public:
		void init(const LV::Variable_Base& _stub) override;

    public:
        inline void set_horizontal_alignment(Horizontal_Alignment _ha) { m_horizontal_alignment = _ha; }
        inline void set_vertical_alignment(Vertical_Alignment _va) { m_vertical_alignment = _va; }

	public:
        void set_text(const std::string& _text);

        inline const std::string& get_text() const { return m_text; }

	};


}

#endif
