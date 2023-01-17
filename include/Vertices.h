#ifndef __VERTICES
#define __VERTICES

#include "Debug/Debug.h"
#include "../include/Buffer.h"

#include "glew.h"

#include <fstream>


namespace LEti {

class Vertices final : public LEti::Buffer
{
public:
	Vertices() : LEti::Buffer() {}
	~Vertices();

	void init(const float* const _coords, unsigned int _count);

public:
	unsigned int get_vertices_count() const;

};

}

#endif
