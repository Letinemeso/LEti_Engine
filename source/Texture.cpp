#include "../include/Texture.h"

#include "../OpenGL/stb_image.h"

using namespace LEti;

Texture::Texture()
{
	picture_valid = false;
	tex_coords_valid = false;
}

Texture::Texture(const char* _path, float* _tex_coords, unsigned int _tex_coords_count)
{
	init(_path, _tex_coords, _tex_coords_count);
}



void Texture::init(const char* _path, float* _tex_coords, unsigned int _tex_coords_count)
{
	set_picture(_path);
	set_texture_coords(_tex_coords, _tex_coords_count);
}

void Texture::set_picture(const char* _path)
{
	picture_valid = false;
	glDeleteTextures(1, &texture_object);

	std::ifstream test(_path, std::ios::in);
	ASSERT(!test.is_open());
	test.close();

	int width, height;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* buffer = stbi_load(_path, &width, &height, nullptr, 4);
	ASSERT(!buffer);

	glGenTextures(1, &texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	stbi_image_free(buffer);

	picture_valid = true;
}

void Texture::set_texture_coords(float* _tex_coords, unsigned int _tex_coords_count)
{
	tex_coords_valid = false;

	delete[] tex_coords;
	tex_coords = nullptr;
	tex_coords_count = 0;

	ASSERT(_tex_coords == nullptr);

	tex_coords_count = _tex_coords_count;
	tex_coords = new float[tex_coords_count];

	for (unsigned int i = 0; i < tex_coords_count; ++i)
		tex_coords[i] = _tex_coords[i];

	tex_coords_valid = true;
}

void Texture::setup_tex_coords_buffer(unsigned int* _buffer, unsigned int _attrib_index)
{
	ASSERT(!tex_coords_valid);

	unsigned int& buffer = *_buffer;

	glDeleteBuffers(1, &buffer);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tex_coords_count + 1, tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(_attrib_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
}



Texture::~Texture()
{
	delete[] tex_coords;
	glDeleteTextures(1, &texture_object);
}


void Texture::use() const
{
	glBindTexture(GL_TEXTURE_2D, texture_object);
}
