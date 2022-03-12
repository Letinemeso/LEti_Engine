#ifndef __TEXTURE
#define __TEXTURE

#include "../include/Debug.h"
#include "../include/Shader.h"

#include "../OpenGL/GLEW/include/glew.h"
#include "../OpenGL/stb_image.h"

#include <fstream>
#include <list>

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
		Texture(const char* _path, float*& _tex_coords, unsigned int _tex_coords_count);

		~Texture();

	public:
		void init(const char* _path, float*& _tex_coords, unsigned int _tex_coords_count);
		void set_picture(const char* _path);
		void set_texture_coords(float* _tex_coords, unsigned int _tex_coords_count);


		float* const get_tc()
		{
			return tex_coords;
		}
	};

}

#endif