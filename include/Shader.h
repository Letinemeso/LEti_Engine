#ifndef __SHADER
#define __SHADER

#include "../include/Debug.h"

#include "../OpenGL/GLEW/include/glew.h"
#include "../OpenGL/GLM/mat4x4.hpp"

#include <iostream>
#include <fstream>


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
		void get_shader_source(const char* _path, char*& _buffer, unsigned int* _size);
		bool shader_debug(unsigned int _shader);
		bool program_debug(unsigned int _program);

	public:
		Shader();
		Shader(Shader&&) = delete;
		void operator=(Shader&&) = delete;
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

	public:
		void init_shader(const char* _v_path, const char* f_path);

		bool operator()() const;

		bool set_uniform(glm::mat4x4& _matrix, const char* _uniform_name);


		unsigned int get_program() const { return program; }	//TODO: remove this 
	};

	static Shader shader;

}	//LEti

#endif