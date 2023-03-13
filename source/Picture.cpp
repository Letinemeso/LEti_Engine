#include "../include/Picture.h"

#include "../OpenGL/stb_image.h"

using namespace LEti;

Picture* LEti::load_picture(const std::string& _path)
{
    stbi_set_flip_vertically_on_load(true);

	int width, height;
    unsigned char* picture_data = stbi_load(_path.c_str(), &width, &height, nullptr, 4);
	L_ASSERT(picture_data);

	Picture* result = new Picture;
	result->m_picture_data = picture_data;
	result->m_width = width;
	result->m_height = height;

    return result;
}



Picture::Picture(Picture&& _from)
{
    m_picture_data = _from.m_picture_data;
    _from.m_picture_data = nullptr;
    m_width = _from.m_width;
    m_height = _from.m_height;
}

Picture::~Picture()
{
//    delete[] m_picture_data;
    stbi_image_free(m_picture_data);
}



int Picture::width() const
{
    return m_width;
}

int Picture::height() const
{
    return m_height;
}

const unsigned char* Picture::data() const
{
    return m_picture_data;
}
