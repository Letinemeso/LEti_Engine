#ifndef __DEFAULT_DRAW_MODULE_2D
#define __DEFAULT_DRAW_MODULE_2D

#include <glew.h>

#include <Shader.h>
#include <Camera_2D.h>
#include <Picture_Manager.h>

#include "Draw_Module_Base.h"


namespace LEti
{

    class Default_Draw_Module_2D_Stub : public Draw_Module_Base_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        std::string texture_name;

        unsigned int tcoords_count = 0;
        float* tcoords = nullptr;

        unsigned int coords_count = 0;
        float* coords = nullptr;

        unsigned int colors_count = 0;
        float* colors = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* /*_product*/) const override;

    public:
        virtual ~Default_Draw_Module_2D_Stub();

    };

    class Default_Draw_Module_2D : public Draw_Module_Base
    {
    public:
        DECLARE_VARIABLE;

    public:
        Default_Draw_Module_2D();
        ~Default_Draw_Module_2D();

        void init_vertices(const float* const _coords, unsigned int _coords_count);
        void init_colors(const float* const _colors, unsigned int _colors_count);
        void init_texture(const Picture* _picture, const float* const tex_coords, unsigned int _tex_coords_count);

        void set_texture(const Picture* _picture);
        void set_texture_coords(const float* _tc, unsigned int _tc_count);

        void move_raw(const glm::vec3& _stride);

    public:
        inline unsigned int gl_draw_mode() const override { return GL_TRIANGLES; }

    };

}


#endif // __DEFAULT_DRAW_MODULE_2D
