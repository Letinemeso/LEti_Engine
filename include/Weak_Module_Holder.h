#pragma once

#include <Module.h>


namespace LEti
{

    class Weak_Module_Holder : public LEti::Module
    {
    public:
        INIT_VARIABLE(LEti::Weak_Module_Holder, LEti::Module)

    private:
        Module* m_module = nullptr;

    private:
        bool m_should_delete_module = false;

    public:
        Weak_Module_Holder();
        ~Weak_Module_Holder();

    public:
        inline void set_should_delete_draw_module(bool _value) { m_should_delete_module = _value; }

    public:
        void set_module(Module* _ptr);

    public:
        inline Module* module() const { return m_module; }

    public:
        void set_transformation_data(Transformation_Data* _data) override;
        void set_transformation_data_prev_state(const Transformation_Data* _data) override;

    public:
        void update(float _dt) override;

    };


    class Weak_Module_Holder_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LEti::Weak_Module_Holder_Stub, LEti::Module_Stub)

        INIT_CHILDS
        ADD_CHILD("module_stub", module_stub)
        CHILDS_END

    public:
        Module_Stub* module_stub = nullptr;

    public:
        ~Weak_Module_Holder_Stub();

    protected:
        INIT_BUILDER_STUB(Weak_Module_Holder)

    };

}
