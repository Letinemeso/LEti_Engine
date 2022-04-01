#include "../include/Vertices.h"

using namespace LEti;

Vertices::~Vertices()
{
	delete[] coordinates;
	delete[] rows;
}



void Vertices::load(const float* const _coords, unsigned int _count)
{
	ASSERT(_coords == nullptr || _count == 0);

	delete[] coordinates;
	count = _count;
	coordinates = new float[count];
	for (unsigned int i = 0; i < count; ++i)
		coordinates[i] = _coords[i];

	delete[] rows;
	row_count = _count / 3;
	rows = new Row[row_count];
	for (unsigned int i = 0; i < row_count; ++i)
		rows[i].init(&coordinates[i * 3]);
}

void Vertices::load(const char* _path)
{

}

void Vertices::setup_vertex_buffer(unsigned int* _buffer, unsigned int _attrib_index)
{
	ASSERT(coordinates == nullptr || count == 0);

	unsigned int& buffer = *_buffer;

	glDeleteBuffers(1, &buffer);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count + 1, coordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(_attrib_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
}



const Vertices::Row& Vertices::operator[](unsigned int _row) const
{
	ASSERT(_row > row_count);

	return rows[_row];
}

unsigned int Vertices::get_vertices_count() const
{
	ASSERT(row_count == 0);

	return row_count;
}