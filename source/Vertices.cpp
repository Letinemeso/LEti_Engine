#include "../include/Vertices.h"

using namespace LEti;

Vertices::~Vertices()
{
	delete[] coordinates;
	delete[] rows;
}



void Vertices::load(float* _coords, unsigned int _count)
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



const Vertices::Row& Vertices::operator[](unsigned int _row) const
{
	ASSERT(_row > row_count);

	return rows[_row];
}