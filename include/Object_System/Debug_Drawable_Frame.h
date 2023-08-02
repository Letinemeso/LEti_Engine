#ifndef __DEBUG_DRAWABLE_FRAME
#define __DEBUG_DRAWABLE_FRAME

#include <Object_System/Object_2D.h>

#include <vector>


namespace LEti
{

    class Debug_Line_Draw_Module : public LR::Default_Draw_Module_2D
    {
    public:
        DECLARE_VARIABLE;

    public:
        inline unsigned int gl_draw_mode() const override { return GL_LINES; }

    };


    class Debug_Line_Draw_Module_Stub : public LR::Default_Draw_Module_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        LV::Variable_Base* M_construct_product() const override { return new Debug_Line_Draw_Module; }

    };



    class Debug_Drawable_Frame : public Object_2D
    {
    public:
        DECLARE_VARIABLE;

    private:
        std::vector<glm::vec3> m_points;
        std::vector<unsigned int> m_sequence;
        bool m_changes_were_made = false;

    public:
        Debug_Drawable_Frame& set_point(unsigned int _index, const glm::vec3& _point);
        Debug_Drawable_Frame& set_sequence_element(unsigned int _index, unsigned int _point_index);
        Debug_Drawable_Frame& clear_points();
        Debug_Drawable_Frame& clear_sequence();

    public:
        void update(float _ratio = 1.0f) override;
    };


    class Debug_Drawable_Frame_Stub : public Object_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        LV::Variable_Base* M_construct_product() const override { return new Debug_Drawable_Frame; }

    };

}

#endif // __DEBUG_DRAWABLE_FRAME
