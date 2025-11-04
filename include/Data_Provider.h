#pragma once

#include <Data_Structures/Vector.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>


namespace LEti
{

    class Data_Provider : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LEti::Data_Provider, LV::Variable_Base)

    public:
        virtual const LDS::Vector<float>& get_data() const = 0;

    };

}
