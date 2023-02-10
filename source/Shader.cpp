#include "../include/Shader.h"

using namespace LEti;


bool Shader::m_initialized = false;

int Shader::m_projection_matrix_uniform = -1;
int Shader::m_transform_matrix_uniform = -1;
int Shader::m_texture_uniform = -1;

unsigned int Shader::m_vertex_shader = 0, Shader::m_fragment_shader = 0;
unsigned int Shader::m_program = 0;


void Shader::M_get_shader_source(const std::string& _path, char*& _result_buffer, unsigned int* _result_size)
{
    std::ifstream file(_path, std::ios::binary);

	L_ASSERT(!(!file.is_open()));

	file.seekg(0, std::ios::end);
	unsigned int size = file.tellg();

	if(_result_size != nullptr) *_result_size = size;

	_result_buffer = new char[size + 1];
	_result_buffer[size] = 0;

	file.seekg(0, std::ios::beg);

    std::string test;

	for (unsigned int i = 0; i < size; ++i)
    {
        _result_buffer[i] = file.get();
        test += _result_buffer[i];
        if(_result_buffer[i] == 0)
            test += "";
    }

	file.close();
}

void Shader::M_shader_debug(unsigned int _shader)
{
	int result = 0;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);
	
	if (result == GL_FALSE)
	{
		int size = 0;
		char log[2048];
		glGetShaderInfoLog(_shader, 2048, &size, log);
        std::cout << log << "\n";
		
        L_ASSERT(false);
	}
}

void Shader::M_program_debug(unsigned int _program)
{
	int result = 0;
	glGetProgramiv(_program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		int size = 0;
		char log[2048];
		glGetProgramInfoLog(_program, 2048, &size, log);
        std::cout << log << "\n";
		
        L_ASSERT(false);
	}
}



void Shader::init_shader(const std::string& _v_path, const std::string& _f_path)
{
    glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);
    glDeleteProgram(m_program);

    m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    m_program = glCreateProgram();

	char* buffer = nullptr;
	unsigned int size = 0;

    M_get_shader_source(_v_path, buffer, &size);
    glShaderSource(m_vertex_shader, 1, &buffer, 0);
    glCompileShader(m_vertex_shader);
    L_DEBUG_FUNC_1ARG(M_shader_debug, m_vertex_shader);
	delete buffer;
    M_get_shader_source(_f_path, buffer, &size);
    glShaderSource(m_fragment_shader, 1, &buffer, 0);
    glCompileShader(m_fragment_shader);
    L_DEBUG_FUNC_1ARG(M_shader_debug, m_fragment_shader);
	delete buffer;


    glAttachShader(m_program, m_vertex_shader);
    glAttachShader(m_program, m_fragment_shader);

    glLinkProgram(m_program);
    L_DEBUG_FUNC_1ARG(M_program_debug, m_program);
    glUseProgram(m_program);

    m_initialized = true;
}

void Shader::set_projection_matrix_uniform_name(const std::string& _name)
{
    m_projection_matrix_uniform = glGetUniformLocation(m_program, _name.c_str());
    L_ASSERT(m_projection_matrix_uniform != -1);
}

void Shader::set_transform_matrix_uniform_name(const std::string& _name)
{
    m_transform_matrix_uniform = glGetUniformLocation(m_program, _name.c_str());
    L_ASSERT(m_transform_matrix_uniform != -1);
}

void Shader::set_texture_uniform_name(const std::string& _name)
{
    m_texture_uniform = glGetUniformLocation(m_program, _name.c_str());
    L_ASSERT(m_texture_uniform != -1);
}


void Shader::set_projection_matrix(glm::mat4x4& _matrix)
{
    L_ASSERT(m_projection_matrix_uniform != -1);

    glUniformMatrix4fv(m_projection_matrix_uniform, 1, false, &_matrix[0][0]);
}

void Shader::set_transform_matrix(glm::mat4x4& _matrix)
{
    L_ASSERT(m_transform_matrix_uniform != -1);

    glUniformMatrix4fv(m_transform_matrix_uniform, 1, false, &_matrix[0][0]);
}

void Shader::set_texture(const LEti::Texture& _texture)
{
    L_ASSERT(m_texture_uniform != -1);
	
    glUniform1i(m_texture_uniform, 0);

    _texture.bind();
}
