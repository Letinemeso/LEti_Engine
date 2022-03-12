#include "../include/Shader.h"

using namespace LEti;

void Shader::get_shader_source(const char* _path, char*& _result_buffer, unsigned int* _result_size)
{
	std::ifstream file(_path, std::ios::in);

	file.seekg(0, std::ios::end);
	unsigned int size = file.tellg();

	if(_result_size != nullptr) *_result_size = size;

	_result_buffer = new char[size + 1];
	_result_buffer[size] = 0;

	file.seekg(0, std::ios::beg);

	for (unsigned int i = 0; i < size; ++i)
		_result_buffer[i] = file.get();

	file.close();
}

void Shader::shader_debug(unsigned int _shader)
{
	int result = 0;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);
	
	if (result == GL_FALSE)
	{
		int size = 0;
		char log[2048];
		glGetShaderInfoLog(_shader, 2048, &size, log);
		std::cout << log << "\n";
		valid = false;
		
		ASSERT(true);
	}
}

void Shader::program_debug(unsigned int _program)
{
	int result = 0;
	glGetProgramiv(_program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		int size = 0;
		char log[2048];
		glGetProgramInfoLog(_program, 2048, &size, log);
		std::cout << log << "\n";
		valid = false;
		
		ASSERT(true);
	}
}



Shader::Shader()
{

}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}



void Shader::init_shader(const char* _v_path, const char* _f_path)
{
	if (initialized) return;

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(program);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	char* buffer = nullptr;
	unsigned int size = 0;

	get_shader_source(_v_path, buffer, &size);
	glShaderSource(vertex_shader, 1, &buffer, 0);
	glCompileShader(vertex_shader);
	DEBUG_FUNC_1ARG(shader_debug, vertex_shader);
	delete buffer;
	get_shader_source(_f_path, buffer, &size);
	glShaderSource(fragment_shader, 1, &buffer, 0);
	glCompileShader(fragment_shader);
	DEBUG_FUNC_1ARG(shader_debug, fragment_shader);
	delete buffer;


	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	DEBUG_FUNC_1ARG(program_debug, program);
	glUseProgram(program);

	initialized = true;
}

void Shader::set_matrix_uniform_name(const char* _name)
{
	matrix_uniform_name = _name;
}

void Shader::set_texture_uniform_name(const char* _name)
{
	texture_uniform_name = _name;
}



bool Shader::operator()() const
{
	return valid && initialized;
}



void Shader::set_matrix(glm::mat4x4& _matrix)
{
	int location = glGetUniformLocation(program, matrix_uniform_name.c_str());
	ASSERT(location == -1);

	glUniformMatrix4fv(location, 1, false, &_matrix[0][0]);
}

void Shader::set_texture(LEti::Texture& _texture)
{
	int location = glGetUniformLocation(program, texture_uniform_name.c_str());
	ASSERT(location == -1);
	
	glUniform1i(location, 0);

	_texture.use();
}