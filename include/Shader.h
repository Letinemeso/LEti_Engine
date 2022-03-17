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
		static bool initialized;
		static bool valid;

	private:
		static unsigned int vertex_shader, fragment_shader;
		static unsigned int program;

	private:
		static std::string matrix_uniform_name, texture_uniform_name;

	private:
		static void get_shader_source(const char* _path, char*& _buffer, unsigned int* _size);
		static void shader_debug(unsigned int _shader);
		static void program_debug(unsigned int _program);

	public:
		Shader() = delete;
		Shader(Shader&&) = delete;
		void operator=(Shader&&) = delete;
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

		~Shader();

	public:
		static void init_shader(const char* _v_path, const char* f_path);
		static void set_matrix_uniform_name(const char* _name);
		static void set_texture_uniform_name(const char* _name);

		//static bool operator()();
		static bool is_valid();

		static void set_matrix(glm::mat4x4& _matrix);
		static void set_texture(const LEti::Texture& _texture);
	};

}	//LEti

#endif