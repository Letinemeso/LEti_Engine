#ifndef __SHADER
#define __SHADER

#include "../Debug.h"
#include "../include/Texture.h"

#include "../OpenGL/GLEW/include/glew.h"
#include "../OpenGL/GLM/mat4x4.hpp"

#include <iostream>
#include <fstream>
#include <string>


namespace LEti {

	class Shader
	{
	private:
		bool initialized = false;
		bool valid = true;

	private:
		unsigned int vertex_shader = 0, fragment_shader = 0;
		unsigned int program = 0;

	private:
		std::string matrix_uniform_name, texture_uniform_name;

	private:
		void get_shader_source(const char* _path, char*& _buffer, unsigned int* _size);
		void shader_debug(unsigned int _shader);
		void program_debug(unsigned int _program);

	public:
		Shader();
		Shader(Shader&&) = delete;
		void operator=(Shader&&) = delete;
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

		~Shader();

	public:
		void init_shader(const char* _v_path, const char* f_path);
		void set_matrix_uniform_name(const char* _name);
		void set_texture_uniform_name(const char* _name);

		bool operator()() const;

		void set_matrix(glm::mat4x4& _matrix);
		void set_texture(LEti::Texture& _texture);

		unsigned int get_program() const { return program; }	//TODO: remove this 
	};

	Shader shader;

}	//LEti

#endif