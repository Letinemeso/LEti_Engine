#ifndef __TEXTURE
#define __TEXTURE

#include "L_Debug/L_Debug.h"
#include "../include/Buffer.h"
#include "../include/Picture.h"

#include "glew.h"

namespace LEti
{

	class Texture final : public LEti::Buffer
	{
	private:
		unsigned int texture_object = 0;
		const Picture* m_picture = nullptr;

	public:
		Texture();
		Texture(const Picture* _picture, float* _tex_coords, unsigned int _tex_coords_count);
		Texture(const LEti::Texture&) = delete;
		Texture(LEti::Texture&&) = delete;
		void operator=(const LEti::Texture&) = delete;
		void operator=(LEti::Texture&&) = delete;

		void init(const Picture* _picture, const float* const _tex_coords, unsigned int _tex_coords_count);
		void set_picture(const Picture* _picture);
		void set_texture_coords(const float* const _tex_coords, unsigned int _tex_coords_count);

		~Texture();

	public:
		const Picture* get_picture() const;

	public:
		void use() const;

	};

}

#endif
