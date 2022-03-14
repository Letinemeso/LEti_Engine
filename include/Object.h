#ifndef __OBJECT
#define __OBJECT

#include "../include/Texture.h"


namespace LEti {

	class Object
	{
	private:
		unsigned int vertex_array = 0;
		unsigned int buffer[2] = { 0 };

		LEti::Texture texture;

	public:
		Object();
		~Object();

	public:
		void init_texture(const char* _tex_path, float* tex_coords, unsigned int _tex_coords_count);
		void init_vertices(float* _coords, unsigned int _coords_count);

	public:
		void draw();
		void update(float _dt);

	};

}	//LEti

#endif