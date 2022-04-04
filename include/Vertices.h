#ifndef __VERTICES
#define __VERTICES

#include "../Debug.h"
#include "../include/Buffer.h"

#include "../OpenGL/GLEW/include/glew.h"

#include <fstream>


namespace LEti {

	class Vertices final : public LEti::Buffer
	{
	public:
		Vertices() : LEti::Buffer() {}
		~Vertices();

		void load(const float* const _coords, unsigned int _count);

	public:
		unsigned int get_vertices_count() const;

	};

}

#endif
