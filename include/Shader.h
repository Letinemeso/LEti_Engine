#ifndef __SHADER
#define __SHADER

#include "L_Debug/L_Debug.h"
#include "../include/Texture.h"

#include "glew.h"
#include "mat4x4.hpp"

#include <iostream>
#include <fstream>
#include <string>


namespace LEti {

	class Shader
	{
	private:
        static bool m_initialized;

	private:
        static unsigned int m_vertex_shader, m_fragment_shader;
        static unsigned int m_program;

    private:
        static int m_projection_matrix_uniform, m_transform_matrix_uniform, m_texture_uniform;

	private:
        static void M_get_shader_source(const std::string& _path, char*& _buffer, unsigned int* _size);
        static void M_shader_debug(unsigned int _shader);
        static void M_program_debug(unsigned int _program);

	public:
		Shader() = delete;
		Shader(Shader&&) = delete;
		void operator=(Shader&&) = delete;
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

	public:
        static void init_shader(const std::string& _v_path, const std::string& f_path);
        static void set_projection_matrix_uniform_name(const std::string& _name);
        static void set_transform_matrix_uniform_name(const std::string& _name);
        static void set_texture_uniform_name(const std::string& _name);

		static void set_projection_matrix(glm::mat4x4& _matrix);
		static void set_transform_matrix(glm::mat4x4& _matrix);
		static void set_texture(const LEti::Texture& _texture);
	};

}	//LEti

#endif
