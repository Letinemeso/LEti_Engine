#ifndef __DEFAULT_DRAW_MODULE_2D
#define __DEFAULT_DRAW_MODULE_2D

#include <glew.h>

#include <Shader.h>
#include <Camera_2D.h>

#include "Draw_Module_Base.h"


namespace LEti
{

    class Default_Draw_Module_2D : public Draw_Module_Base
    {
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
        inline unsigned int gl_draw_mode() const override { return GL_TRIANGLES; }

    };

}


#endif // __DEFAULT_DRAW_MODULE_2D
