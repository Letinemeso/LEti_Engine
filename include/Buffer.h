#ifndef __BUFFER
#define __BUFFER

#include "glew.h"

#include "L_Debug/L_Debug.h"


namespace LEti
{

    class Buffer
    {
    protected:
	class float_container final
	{
	private:
	    friend class Buffer;

	private:
	    const unsigned int* buffer = nullptr;
	    float* buffer_data = nullptr;
	    unsigned int last_requested_index = 0;

    private:
        void M_update_buffer();

	public:
	    void operator=(float _f);
	    void operator+=(float _f);
	    void operator-=(float _f);
        void operator*=(float _f);
        void operator/=(float _f);
	    float operator*() const;
	    operator float() const;
	};
	float_container fc;

    public:
	const unsigned int* vertex_array = nullptr;

    protected:
	unsigned int buffer = 0;
	float* buffer_data = nullptr;
	unsigned int buffer_size = 0;

    public:
	Buffer();
	virtual ~Buffer();

	virtual void allocate_memory(unsigned int _size);
	virtual void free_memory();
	virtual void resize(unsigned int _new_size);

	virtual void copy_array(const float* _data, unsigned int _count, unsigned int _offset = 0);

	virtual void setup_buffer(unsigned int _attrib_index, unsigned int _floats_per_vertex);

	float_container& operator[](unsigned int _index);
	float operator[](unsigned int _index) const;

	unsigned int size() const;

	void bind() const;

    };


}   /*LEti*/

#endif
