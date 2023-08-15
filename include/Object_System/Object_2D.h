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
        inline Transformation_Data& current_state() { return m_current_state; }
        inline const Transformation_Data& current_state() const { return m_current_state; }
        inline Transformation_Data& previous_state() { return m_previous_state; }
        inline const Transformation_Data& previous_state() const { return m_previous_state; }

    public:
        void add_module(Module* _module);
        void remove_module(Module* _module);    //  this does not delete a module (if you have pointer, you should delete it yourself)
        void remove_all_modules();  //  this deletes all modules

    public:
        void update_previous_state();
        void update() override;

    };

}


#endif // __OBJECT_2D
