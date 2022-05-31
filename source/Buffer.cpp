#include "../include/Buffer.h"

using namespace LEti;


void Buffer::float_container::operator=(float _f)
{
	buffer_data[last_requested_index] = _f;
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * last_requested_index, sizeof(float), &_f);
}

float Buffer::float_container::operator*() const
{
	return buffer_data[last_requested_index];
}



Buffer::Buffer()
{
	fc.buffer = &buffer;
}

Buffer::~Buffer()
{
	free_memory();
}



void Buffer::allocate_memory(unsigned int _size)
{
	ASSERT(_size == 0 || vertex_array == nullptr);

	free_memory();

	buffer_data = new float[_size];
	fc.buffer_data = buffer_data;
	for (unsigned int i = 0; i < _size; ++i)
		buffer_data[i] = 0.0f;
	buffer_size = _size;

	glBindVertexArray(*vertex_array);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (buffer_size + 1), nullptr, GL_DYNAMIC_DRAW);
}

void Buffer::free_memory()
{
	delete[] buffer_data;
	buffer_data = nullptr;
	fc.buffer_data = nullptr;
	buffer_size = 0;

	glDeleteBuffers(1, &buffer);
}

void Buffer::resize(unsigned int _new_size)
{
	ASSERT(_new_size == 0 || vertex_array == nullptr);
	glBindVertexArray(*vertex_array);

	float* tempf = new float[_new_size];
	if (buffer_size < _new_size)
	{
		for (unsigned int i = 0; i < buffer_size; ++i)
			tempf[i] = buffer_data[i];
		for (unsigned int i = buffer_size; i < _new_size; ++i)
			tempf[i] = 0.0f;
	}
	else
	{
		for (unsigned int i = 0; i < _new_size; ++i)
			tempf[i] = buffer_data[i];
	}

	glDeleteBuffers(1, &buffer);
	delete[] buffer_data;

	buffer_size = _new_size;
	buffer_data = tempf;
	fc.buffer_data = tempf;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (buffer_size + 1), buffer_data, GL_DYNAMIC_DRAW);
}



void Buffer::copy_array(const float* _data, unsigned int _count, unsigned int _offset)
{
	ASSERT(_offset + _count > buffer_size || _data == nullptr || _count == 0 || vertex_array == nullptr);

	for(unsigned int i=0; i<_count; ++i)
		buffer_data[i + _offset] = _data[i];

	glBindVertexArray(*vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * _offset, sizeof(float) * _count, _data);
}



void Buffer::setup_buffer(unsigned int _attrib_index, unsigned int _floats_per_vertex)
{
	ASSERT(buffer == 0 || buffer_size == 0 || buffer_data == nullptr || vertex_array == nullptr);

	glBindVertexArray(*vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(_attrib_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * _floats_per_vertex, nullptr);
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



unsigned int Buffer::size() const
{
	return buffer_size;
}



void Buffer::bind() const
{
	ASSERT(buffer == 0 || buffer_size == 0 || buffer_data == nullptr || vertex_array == nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}
