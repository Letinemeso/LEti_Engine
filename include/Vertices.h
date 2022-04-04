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

                void setup_vertex_buffer(unsigned int _attrib_index);

        public:
                unsigned int get_vertices_count() const;

	};

}

#endif
