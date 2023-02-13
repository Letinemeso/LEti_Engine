#ifndef __SHADER
#define __SHADER

#include "L_Debug/L_Debug.h"
#include "../include/Texture.h"

#include "glew.h"
#include "mat4x4.hpp"

#include <iostream>
#include <fstream>
#include <string>


namespace LEti
{

	class Shader
    {
	private:
        unsigned int m_vertex_shader = 0, m_fragment_shader = 0;
        unsigned int m_program = 0;

    private:
        int m_projection_matrix_uniform = -1, m_transform_matrix_uniform = -1, m_texture_uniform = -1;

    public:
        Shader();
        Shader(Shader&& _other);
        void operator=(Shader&& _other);
        Shader(const Shader&) = delete;
        void operator=(const Shader&) = delete;

        ~Shader();

	private:
        void M_get_shader_source(const std::string& _path, char*& _buffer, unsigned int* _size) const;
        void M_shader_debug(unsigned int _shader) const;
        void M_program_debug(unsigned int _program) const;

	public:
        void init(const std::string& _v_path, const std::string& _f_path);
        void set_projection_matrix_uniform(const std::string& _name);
        void set_transform_matrix_uniform(const std::string& _name);
        void set_texture_uniform(const std::string& _name);

        void set_projection_matrix(const glm::mat4x4& _matrix) const;
        void set_transform_matrix(const glm::mat4x4& _matrix) const;
        void set_texture(const LEti::Texture& _texture) const;
	};

}	//LEti

#endif
