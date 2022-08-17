#ifndef __DEFAULT_DRAW_MODULE_2D
#define __DEFAULT_DRAW_MODULE_2D

#include <glew.h>

#include "../../Texture.h"
#include "../../Vertices.h"
#include "../../Shader.h"
#include <include/Camera.h>

#include "Draw_Module_Base.h"


namespace TEST {

	class Default_Draw_Module_2D : public Draw_Module_Base
	{
	private:
		bool m_visible = true;

		unsigned int m_vertex_array = 0;

		LEti::Texture m_texture;
		LEti::Vertices m_vertices;

	public:
		Default_Draw_Module_2D();
		~Default_Draw_Module_2D();

	public:
		void init_texture(const char* _picture_name, const float* const tex_coords, unsigned int _tex_coords_count);
		void init_vertices(const float* const _coords, unsigned int _coords_count);

	public:
		void draw(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4 _scale) const override;

	public:
		LEti::Texture& get_texture();
		LEti::Vertices& get_vertices();

	};

}


#endif // __DEFAULT_DRAW_MODULE_2D
