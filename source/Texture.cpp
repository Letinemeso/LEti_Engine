#include "../include/Texture.h"

#include "../OpenGL/stb_image.h"

using namespace LEti;

Texture::Texture()
{

}

Texture::Texture(const char* _path, float* _tex_coords, unsigned int _tex_coords_count)
{
	init(_path, _tex_coords, _tex_coords_count);
}



void Texture::init(const char* _path, const float* const _tex_coords, unsigned int _tex_coords_count)
{
	set_picture(_path);
	set_texture_coords(_tex_coords, _tex_coords_count);
}

void Texture::set_picture(const char* _path)
{
	glDeleteTextures(1, &texture_object);

	int width, height;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* buffer = stbi_load(_path, &width, &height, nullptr, 4);
	ASSERT(buffer == nullptr);

	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	stbi_image_free(buffer);
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


void Texture::use() const
{
	ASSERT(texture_object == 0 || buffer_size == 0 || buffer == 0 || buffer_data == 0);
	glBindTexture(GL_TEXTURE_2D, texture_object);
}
