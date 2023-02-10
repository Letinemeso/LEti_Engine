#ifndef COLORS_H
#define COLORS_H

#include "L_Debug/L_Debug.h"
#include "../include/Buffer.h"

#include "glew.h"

#include <fstream>


namespace LEti
{

    class Colors final : public LEti::Buffer
    {
    public:
        Colors() : LEti::Buffer() {}
        ~Colors();

        void init(const float* const _coords, unsigned int _count);

    public:
        unsigned int vertices_count() const;

    };

}

#endif // COLORS_H
