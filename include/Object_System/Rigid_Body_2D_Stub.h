#ifndef RIGID_BODY_2D_H
#define RIGID_BODY_2D_H

#include <Object_System/Object_2D.h>
//#include <Modules/Physics_Module__Rigid_Body_2D.h>


namespace LEti
{

    class Rigid_Body_2D_Stub : public Object_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    protected:
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}

#endif // RIGID_BODY_2D_H
