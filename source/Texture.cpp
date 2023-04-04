#include "../include/Texture.h"

using namespace LEti;


Texture::Texture()
{

}

Texture::Texture(const Picture* _picture, float* _tex_coords, unsigned int _tex_coords_count)
{
	init(_picture, _tex_coords, _tex_coords_count);
}



void Texture::init(const Picture* _picture, const float* const _tex_coords, unsigned int _tex_coords_count)
{
	set_picture(_picture);
	set_texture_coords(_tex_coords, _tex_coords_count);
}

void Texture::set_picture(const Picture* _picture)
{
	glDeleteTextures(1, &m_texture_object);

	m_picture = _picture;

	glGenTextures(1, &m_texture_object);
	glBindTexture(GL_TEXTURE_2D, m_texture_object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_picture->width(), m_picture->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_picture->data());
}

void Texture::set_texture_coords(const float* const _tex_coords, unsigned int _tex_coords_count)
{
	free_memory();
	allocate_memory(_tex_coords_count);
	copy_array(_tex_coords, _tex_coords_count);
}

void Texture::reconfigure_texture_coords()
{
    L_ASSERT(m_picture);
    L_ASSERT(size() > 0);

    for(unsigned int i=0; i<size(); i += 2)
    {
        glm::vec2 vertex;
        vertex.x = (*this)[i];
        vertex.y = (*this)[i + 1];
        m_picture->convert_texture_coords_vertex(vertex);
        (*this)[i] = vertex.x;
        (*this)[i + 1] = vertex.y;
    }
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture_object);
}



const Picture* Texture::get_picture() const
{
	return m_picture;
}



void Texture::bind() const
{
	L_ASSERT(!(m_texture_object == 0 || buffer_size == 0 || buffer == 0 || buffer_data == 0));
	glBindTexture(GL_TEXTURE_2D, m_texture_object);
}
