#include "../include/Texture.h"

using namespace LEti;

Texture::Texture()
{

}

Texture::Texture(const char* _picture_name, float* _tex_coords, unsigned int _tex_coords_count)
{
	init(_picture_name, _tex_coords, _tex_coords_count);
}



void Texture::init(const char* _picture_name, const float* const _tex_coords, unsigned int _tex_coords_count)
{
	set_picture(_picture_name);
	set_texture_coords(_tex_coords, _tex_coords_count);
}

void Texture::set_picture(const char* _picture_name)
{
	glDeleteTextures(1, &texture_object);

	m_picture_name = _picture_name;
	m_picture = &LEti::Resource_Loader::get_picture(_picture_name);

	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_picture->width(), m_picture->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_picture->data());
}

void Texture::set_texture_coords(const float* const _tex_coords, unsigned int _tex_coords_count)
{
	free_memory();
	allocate_memory(_tex_coords_count);
	copy_array(_tex_coords, _tex_coords_count);
}


Texture::~Texture()
{
	glDeleteTextures(1, &texture_object);
}



const Picture& Texture::get_current_picture() const
{
	return *m_picture;
}

const std::string& Texture::get_current_picture_name() const
{
	return m_picture_name;
}



void Texture::use() const
{
	L_ASSERT(!(texture_object == 0 || buffer_size == 0 || buffer == 0 || buffer_data == 0));
	glBindTexture(GL_TEXTURE_2D, texture_object);
}
