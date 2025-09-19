#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Transformation_Data.h>


namespace LEti
{

    class Object;


    class Module : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LEti::Module, LV::Variable_Base)

    private:
        Transformation_Data* m_transformation_data = nullptr;
        const Transformation_Data* m_transformation_data_prev_state = nullptr;

        Object* m_parent_object = nullptr;

    public:
        Module();
        ~Module();

    public:
        virtual void set_transformation_data(Transformation_Data* _data) { m_transformation_data = _data; }
        virtual void set_transformation_data_prev_state(const Transformation_Data* _data) { m_transformation_data_prev_state = _data; }
        inline void set_parent_object(Object* _ptr) { m_parent_object = _ptr; M_on_parent_object_set(); }

        inline Transformation_Data* transformation_data() const { return m_transformation_data; }
        inline const Transformation_Data* transformation_data_prev_state() const { return m_transformation_data_prev_state; }
        inline Object* parent_object() const { return m_parent_object; }

    protected:
        virtual void M_on_parent_object_set() { }

    public:
        virtual void update_prev_state() { }
        virtual void update(float _dt) = 0;

    };


    class Module_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LEti::Module_Stub, LV::Builder_Stub)

    public:
        INIT_BUILDER_STUB(Module)

    };

}
