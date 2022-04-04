#include "../include/Vertices.h"

using namespace LEti;

Vertices::~Vertices()
{
//	delete[] coordinates;
//	delete[] rows;
}



void Vertices::load(const float* const _coords, unsigned int _count)
{
    ASSERT(_coords == nullptr || _count == 0);

    allocate_memory(_count);
    copy_array(_coords, _count);
}



void Vertices::setup_vertex_buffer(unsigned int _attrib_index)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(_attrib_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
}



unsigned int Vertices::get_vertices_count() const
{
    ASSERT(buffer_size == 0);

    return buffer_size / 3;
}
