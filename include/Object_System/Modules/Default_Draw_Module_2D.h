#ifndef __DEFAULT_DRAW_MODULE_2D
#define __DEFAULT_DRAW_MODULE_2D

#include <glew.h>

#include <Texture.h>
#include <Vertices.h>
#include <Colors.h>
#include <Shader.h>
#include <Camera_2D.h>

#include "Draw_Module_Base.h"


namespace LEti
{

    class Default_Draw_Module_2D : public Draw_Module_Base
    {
    protected:
        bool m_visible = true;

        unsigned int m_vertex_array = 0;

        LEti::Texture m_texture;
        LEti::Vertices m_vertices;
        LEti::Colors m_colors;

    public:
        Default_Draw_Module_2D();
        ~Default_Draw_Module_2D();

    public:
        void init_texture(const Picture* _picture, const float* const tex_coords, unsigned int _tex_coords_count);
        void init_vertices(const float* const _coords, unsigned int _coords_count);
        void init_colors(const float* const _colors, unsigned int _colors_count);

        void set_texture(const Picture* _picture);
        void set_texture_coords(const float* _tc, unsigned int _tc_count);

    public:
        void draw(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4 _scale) const override;

    public:
        inline LEti::Texture& texture() { return m_texture; }
        inline LEti::Vertices& vertices() { return m_vertices; }
        inline LEti::Colors& colors() { return m_colors; }

    };

}


#endif // __DEFAULT_DRAW_MODULE_2D
