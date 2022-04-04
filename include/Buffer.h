#ifndef __BUFFER
#define __BUFFER

#include "../OpenGL/GLEW/include/glew.h"

#include "../Debug.h"


namespace LEti {

    class Buffer final
    {
    private:
        class float_container final
        {
        private:
            friend class Buffer;
        private:
            unsigned int* buffer = nullptr;
            float* buffer_data = nullptr;
            mutable unsigned int last_requested_index = 0;
        public:
            void operator=(float _f);
            float operator*() const;
        };
        float_container fc;

    private:
        unsigned int buffer = 0;

        float* buffer_data = nullptr;
        unsigned int buffer_size = 0;

    public:
        Buffer() {}
        ~Buffer();

        void allocate_memory(unsigned int _size);
        void free_memory();

        void copy_array(const float* _data, unsigned int _count, unsigned int _offset = 0);

        float_container& operator[](unsigned int _index);
        float operator[](unsigned int _index) const;

    };


}   /*LEti*/

#endif
