#ifndef __OBJECT_2D
#define __OBJECT_2D

#include <Data_Structures/List.h>

#include <Builder_Stub.h>

#include <Object_System/Object_Base.h>
#include <Transformation_Data.h>
#include <Module.h>



namespace LEti
{

    class Object_2D_Stub : public LV::Builder_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        glm::vec3 position, scale, rotation_axis;
        float rotation_angle;

//        bool enable_draw_module = false;
//        LR::Draw_Module_Base_Stub* draw_module = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    public:
        ~Object_2D_Stub();

    };

    class Object_2D : public Object_Base
    {
    public:
        DECLARE_VARIABLE;

    protected:
        LDS::List<Module*> m_modules;

    protected:
        Transformation_Data m_current_state;
        Transformation_Data m_previous_state;

    public:
        Object_2D();
        ~Object_2D();

    public:
        void set_pos(const glm::vec3& _position);
        void move(const glm::vec3& _stride);

        void set_rotation_axis(const glm::vec3& _axis);
        void set_rotation_angle(float _angle);
        void rotate(float _angle);

        void set_scale(const glm::vec3& _scale);
        void set_scale(float _scale);

    public:
        glm::vec3 get_pos() const;
        glm::vec3 get_scale() const;
        glm::vec3 get_rotation_axis() const;
        float get_rotation_angle() const;

        glm::vec3 get_pos_prev() const;
        glm::vec3 get_scale_prev() const;
        glm::vec3 get_rotation_axis_prev() const;
        float get_rotation_angle_prev() const;

    public:
        void add_module(Module* _module);
        void remove_module(Module* _module);    //  this does not delete a module (if you have pointer, you should delete it yourself)
        void remove_all_modules();  //  this deletes all modules

    public:
        virtual void revert_to_previous_state();
        virtual void update_previous_state();
        void update() override;
        void draw(const LR::Renderer &_renderer) const override;

    };

}


#endif // __OBJECT_2D
