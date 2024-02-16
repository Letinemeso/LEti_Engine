#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Transformation_Data.h>


namespace LEti
{

    class Module : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LEti::Module, LV::Variable_Base)

    private:
        Transformation_Data* m_transformation_data = nullptr;
        const Transformation_Data* m_transformation_data_prev_state = nullptr;

    public:
        Module();
        ~Module();

    public:
        virtual void set_transformation_data(Transformation_Data* _data) { m_transformation_data = _data; }
        virtual void set_transformation_data_prev_state(const Transformation_Data* _data) { m_transformation_data_prev_state = _data; }

        inline Transformation_Data* transformation_data() const { return m_transformation_data; }
        inline const Transformation_Data* transformation_data_prev_state() const { return m_transformation_data_prev_state; }

    public:
        virtual void update_prev_state() { }
        virtual void update(float _dt) = 0;

    };



    class Module_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LEti::Module, LV::Builder_Stub)

    };

}
