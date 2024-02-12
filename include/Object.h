#pragma once

#include <Stuff/Function_Wrapper.h>
#include <Data_Structures/List.h>

#include <Builder_Stub.h>

#include <Transformation_Data.h>
#include <Module.h>



namespace LEti
{

    class Object : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

    protected:
        Transformation_Data m_current_state;
        Transformation_Data m_previous_state;

    protected:
        LDS::List<Module*> m_modules;
        LST::Function<void(Object*)> m_on_update_func;

    public:
        Object();
        virtual ~Object();

    public:
        inline Transformation_Data& current_state() { return m_current_state; }
        inline const Transformation_Data& current_state() const { return m_current_state; }
        inline Transformation_Data& previous_state() { return m_previous_state; }
        inline const Transformation_Data& previous_state() const { return m_previous_state; }

    public:
        inline void set_on_update_func(const LST::Function<void(Object*)>& _func) { m_on_update_func = _func; }

    public:
        void add_module(Module* _module);
        void remove_module(Module* _module);    //  this does not delete a module (if you have pointer, you should delete it yourself)
        void remove_all_modules();  //  this deletes all modules

    public:
        inline const LDS::List<Module*>& modules() const { return m_modules; }

    public:
        void update_previous_state();
        virtual void update(float _dt);

    };



    class Object_Stub : public LV::Builder_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        glm::vec3 position, scale, rotation_angles;

    public:
        using Module_Stubs_Map = LDS::Map<std::string, Module_Stub*>;

    public:
        Module_Stubs_Map module_stubs;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    public:
        ~Object_Stub();

    };

}
