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
        INIT_VARIABLE(LEti::Object, LV::Variable_Base)

    protected:
        Transformation_Data m_current_state;
        Transformation_Data m_previous_state;

    public:
        using Modules_List = LDS::List<Module*>;

    public:
        using On_Update_Func = LST::Function<void(Object*)>;

    protected:
        Modules_List m_modules;
        On_Update_Func m_on_update_func;

    public:
        Object();
        virtual ~Object();

    public:
        inline Transformation_Data& current_state() { return m_current_state; }
        inline const Transformation_Data& current_state() const { return m_current_state; }
        inline Transformation_Data& previous_state() { return m_previous_state; }
        inline const Transformation_Data& previous_state() const { return m_previous_state; }

    public:
        inline void set_on_update_func(const On_Update_Func& _func) { m_on_update_func = _func; }

    public:
        void add_module(Module* _module);
        void remove_module(Module* _module);    //  this does not delete a module (if you have pointer, you should delete it yourself)
        void remove_all_modules();  //  this deletes all modules

    public:
        inline const Modules_List& modules() const { return m_modules; }

    public:
        template<typename Module_Type>
        void process_logic_for_modules_of_type(const LST::Function<void(Module_Type*)>& _logic);
        template<typename Module_Type>
        void process_logic_for_modules_of_type(const LST::Function<void(const Module_Type*)>& _logic) const;
        template<typename Module_Type>
        Module_Type* get_module_of_type(unsigned int _skip_amount = 0) const;

    public:
        void update_previous_state();
        virtual void update(float _dt);

    };

    template<typename Module_Type>
    void Object::process_logic_for_modules_of_type(const LST::Function<void(Module_Type*)>& _logic)
    {
        L_ASSERT(_logic);

        for(Modules_List::Iterator module_it = m_modules.begin(); !module_it.end_reached(); ++module_it)
        {
            Module_Type* requested_type_module = LV::cast_variable<Module_Type>(*module_it);
            if(!requested_type_module)
                continue;

            _logic(requested_type_module);
        }
    }

    template<typename Module_Type>
    void Object::process_logic_for_modules_of_type(const LST::Function<void(const Module_Type*)>& _logic) const
    {
        L_ASSERT(_logic);

        for(Modules_List::Const_Iterator module_it = m_modules.begin(); !module_it.end_reached(); ++module_it)
        {
            const Module_Type* requested_type_module = LV::cast_variable<Module_Type>(*module_it);
            if(!requested_type_module)
                continue;

            _logic(requested_type_module);
        }
    }

    template<typename Module_Type>
    Module_Type* Object::get_module_of_type(unsigned int _skip_amount) const
    {
        for(Modules_List::Const_Iterator module_it = m_modules.begin(); !module_it.end_reached(); ++module_it)
        {
            Module_Type* requested_type_module = LV::cast_variable<Module_Type>(*module_it);
            if(!requested_type_module)
                continue;

            if(_skip_amount == 0)
                return requested_type_module;

            --_skip_amount;
        }

        return nullptr;
    }


    class Object_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LEti::Object_Stub, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(glm::vec3, position)
        ADD_FIELD(glm::vec3, scale)
        ADD_FIELD(glm::vec3, rotation_angles)
        FIELDS_END

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("Module__*", &module_stubs)
        CHILDS_LISTS_END

    public:
        glm::vec3 position, scale, rotation_angles;

    public:
        LV::Variable_Base::Childs_List module_stubs;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    public:
        ~Object_Stub();

    };

}
