#include "../include/Object.h"

using namespace LEti;

Object::Object()
{
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

Object::~Object()
{
	glDeleteBuffers(2, buffer);
	glDeleteVertexArrays(1, &vertex_array);
}



void Object::init_texture(const char* _tex_path, float* tex_coords, unsigned int _tex_coords_count)
{
	texture.init(_tex_path, tex_coords, _tex_coords_count);
	texture.setup_tex_coords_buffer(&buffer[1], 1);
}

void Object::init_vertices(float* _coords, unsigned int _coords_count)
{
	glBindVertexArray(vertex_array);

	//glGenBuffers(1)
}
