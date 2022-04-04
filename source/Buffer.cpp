#include "../include/Buffer.h"

using namespace LEti;


void Buffer::float_container::operator=(float _f)
{
    buffer[last_requested_index] = _f;
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 1, &_f);
}

float Buffer::float_container::operator*() const
{
    return buffer[last_requested_index];
}



Buffer::~Buffer()
{
    free_memory();
}



void Buffer::allocate_memory(unsigned int _size)
{
    ASSERT(_size == 0);

    buffer_data = new float[_size];
    fc.buffer_data = buffer_data;
    for(unsigned int i=0; i<_size; ++i)
        buffer_data[i] = 0.0f;
    buffer_size = _size;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (_size+1), nullptr, GL_DYNAMIC_DRAW);
}

void Buffer::free_memory()
{
    delete[] buffer_data;
    buffer_data = nullptr;
    fc.buffer_data = nullptr;
    buffer_size = 0;

    glDeleteBuffers(1, &buffer);
}



void Buffer::copy_array(const float *_data, unsigned int _count, unsigned int _offset)
{
    ASSERT(_offset + _count > buffer_size || _data == nullptr);

    for(unsigned int i=_offset; i<_offset + _count; ++i)
        buffer_data[i] = _data[i];

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData(GL_ARRAY_BUFFER, _offset, sizeof(float) * _count, _data);
}



Buffer::float_container& Buffer::operator[](unsigned int _index)
{
    ASSERT(_index >= buffer_size);

    fc.last_requested_index = _index;
    return fc;
}

float Buffer::operator[](unsigned int _index) const
{
    ASSERT(_index >= buffer_size);
    return buffer_data[_index];
}
