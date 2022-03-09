#ifndef __SHADER
#define __SHADER

#include "../OpenGL/GLEW/include/glew.h"

#include <iostream>
#include <fstream>


namespace LEti {

	class Shader
	{
	private:
		static unsigned int vertex_shader, fragment_shader;
		static unsigned int program;

	private:
		static void get_shader_source(const char* _path, char* _buffer, unsigned int* _size);

	public:
		Shader() = delete;
		Shader(Shader&&) = delete;
		void operator=(Shader&&) = delete;
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

	public:
		static void init();
		static void create_shader(const char* _v_path, const char* f_path);

	};

}	//LEti


#endif