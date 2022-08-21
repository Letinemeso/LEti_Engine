#include "../include/Vertices.h"

using namespace LEti;


Vertices::~Vertices()
{

}



void Vertices::init(const float* const _coords, unsigned int _count)
{
	ASSERT(_coords == nullptr || _count == 0);

	free_memory();
	allocate_memory(_count);
	copy_array(_coords, _count);
}



unsigned int Vertices::get_vertices_count() const
{
	ASSERT(buffer_size == 0);

	return buffer_size / 3;
}
