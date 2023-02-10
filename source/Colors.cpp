#include <Colors.h>

using namespace LEti;


Colors::~Colors()
{

}



void Colors::init(const float* const _coords, unsigned int _count)
{
    L_ASSERT(_coords != nullptr && _count != 0 && _count % 4 == 0);

    free_memory();
    allocate_memory(_count);
    copy_array(_coords, _count);
}



unsigned int Colors::vertices_count() const
{
    L_ASSERT(!(buffer_size == 0));

    return buffer_size / 4;
}

