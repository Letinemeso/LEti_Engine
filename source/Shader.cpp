#include "../include/Shader.h"

using namespace LEti;

void Shader::get_shader_source(const char* _path, char* _buffer, unsigned int* _size)
{
	std::ifstream file(_path, std::ios::in);

	file.seekg(0, std::ios::end);
	unsigned int size = file.tellg();

	*_size = size;
	_buffer = new char[size + 1];
	_buffer[size] = 0;

	file.seekg(0, std::ios::beg);

	for (unsigned int i = 0; i < size; ++i)
		_buffer[i] = file.get();

	file.close();
}



void Shader::init()
{
	vertex_shader = 0;
	fragment_shader = 0;
	program = 0;
}

void Shader::create_shader(const char* _v_path, const char* f_path)
{
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	char* buffer = nullptr;
	unsigned int size = 0;

	get_shader_source(_v_path, buffer, &size);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);



}