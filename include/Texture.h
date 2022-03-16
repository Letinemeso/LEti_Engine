#ifndef __TEXTURE
#define __TEXTURE

#include "../Debug.h"

#include "../OpenGL/GLEW/include/glew.h"

#include <fstream>

namespace LEti {

	class Texture
	{
	private:
		bool picture_valid	  = false,
			 tex_coords_valid = false;

	private:
		unsigned int texture_object = 0;
		float* tex_coords = nullptr;
		unsigned int tex_coords_count = 0;

	public:
		Texture();
		Texture(const char* _path, float* _tex_coords, unsigned int _tex_coords_count);
		Texture(const LEti::Texture&) = delete;
		Texture(LEti::Texture&&) = delete;
		void operator=(const LEti::Texture&) = delete;
		void operator=(LEti::Texture&&) = delete;

		void init(const char* _path, float* _tex_coords, unsigned int _tex_coords_count);
		void set_picture(const char* _path);
		void set_texture_coords(float* _tex_coords, unsigned int _tex_coords_count);

		void setup_tex_coords_buffer(unsigned int* _buffer, unsigned int _attrib_index);

		~Texture();

	public:
		void use() const;

	};

}

#endif