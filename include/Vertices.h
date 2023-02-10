#ifndef __VERTICES
#define __VERTICES

#include "L_Debug/L_Debug.h"
#include "../include/Buffer.h"

#include "glew.h"

#include <fstream>


namespace LEti
{

    class Vertices final : public LEti::Buffer
    {
    public:
    Vertices() : LEti::Buffer() {}
    ~Vertices();

    void init(const float* const _coords, unsigned int _count);

    public:
    unsigned int vertices_count() const;

    };

}

#endif
