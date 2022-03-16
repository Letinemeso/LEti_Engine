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
	glBindVertexArray(vertex_array);
	texture.init(_tex_path, tex_coords, _tex_coords_count);
	texture.setup_tex_coords_buffer(&buffer[1], 1);
}

void Object::init_vertices(float* _coords, unsigned int _coords_count)
{
	glBindVertexArray(vertex_array);
	vertices.load(_coords, _coords_count);
	vertices.setup_vertex_buffer(&buffer[0], 0);

	//glGenBuffers(1)
}



void Object::draw()
{
	glBindVertexArray(vertex_array);
	//texture.use();
	LEti::Shader::set_texture(texture);
	glDrawArrays(GL_TRIANGLES, 0, vertices.get_vertices_count());
}