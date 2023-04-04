#ifndef BUILDER_STUB_H
#define BUILDER_STUB_H

#include <Variable_Base.h>


namespace LEti
{

    class Builder_Stub : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

    protected:
        virtual LV::Variable_Base* M_construct_product() const = 0;
        virtual void M_init_constructed_product(LV::Variable_Base* /*_product*/) const {}

    public:
        virtual ~Builder_Stub();
        LV::Variable_Base* construct() const;

    };

}

#endif // BUILDER_STUB_H
