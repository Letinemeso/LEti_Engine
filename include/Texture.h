#ifndef __TEXTURE
#define __TEXTURE

#include "../Debug.h"
#include "../include/Buffer.h"
#include "../include/Picture.h"
#include "../include/Resource_Loader.h"

#include "glew.h"

#include <fstream>

namespace LEti {

    class Texture final : public LEti::Buffer
    {
    private:
	unsigned int texture_object = 0;
	const Picture* m_picture = nullptr;
	std::string m_picture_name;

    public:
	Texture();
	Texture(const char* _picture_name, float* _tex_coords, unsigned int _tex_coords_count);
	Texture(const LEti::Texture&) = delete;
	Texture(LEti::Texture&&) = delete;
	void operator=(const LEti::Texture&) = delete;
	void operator=(LEti::Texture&&) = delete;

	void init(const char* _picture_name, const float* const _tex_coords, unsigned int _tex_coords_count);
	void set_picture(const char* _picture_name);
	void set_texture_coords(const float* const _tex_coords, unsigned int _tex_coords_count);

	~Texture();

    public:
	const Picture& get_current_picture() const;
	const std::string& get_current_picture_name() const;

    public:
	void use() const;

    };

}

#endif
