#ifndef __VERTICES
#define __VERTICES

#include "../Debug.h"
#include "../OpenGL/GLEW/include/glew.h"

#include <fstream>


namespace LEti {

	class Vertices
	{
	private:
		class Row
		{
		private:
			const float* ptr = nullptr;
		public:
			Row() {}
			Row(const Row&) = delete;
			Row(Row&&) = delete;
			void operator=(const Row&) = delete;
			void operator=(Row&&) = delete;

			void init(float* _ptr) { ptr = _ptr; }
			float operator[](unsigned int _col) const { ASSERT(_col > 2); return ptr[_col]; }
		};

	private:
		float* coordinates = nullptr;
		unsigned int count = 0;
		
		Row* rows = nullptr;
		unsigned int row_count = 0;

	public:
		Vertices() {}
		~Vertices();

		void load(float* _coords, unsigned int _count);
		void load(const char* _path);

		void setup_vertex_buffer(unsigned int* _buffer, unsigned int _attrib_index);

	public:
		const Row& operator[](unsigned int _row) const;
		unsigned int get_vertices_count() const;

	};

}

#endif