#ifndef __TEXT_FIELD
#define __TEXT_FIELD

//#include "../include/Object.h"
#include <include/Object_System/Object_2D.h>

#include <string>

namespace std
{
	using unsigned_string = basic_string<unsigned char>;
}

namespace LEti
{
	class Text_Field : public Object_2D
	{
	private:
		const std::unsigned_string* sequence = nullptr;
		std::pair<const float*, unsigned int> text_tex_coords;

		std::map<unsigned char, const float*> sequence_map;

	private:
		float width = 0.0f, height = 0.0f, actual_symbol_width = 0.0f;
		std::unsigned_string text;

	public:
		Text_Field();

	public:
		void init(const char* _object_name) override;

	public:
		void set_text(const char* _text);
		const std::unsigned_string& get_text() const;

	public:
		void draw() const override;

	};


}

#endif
