#include <Object_System/Rigid_Body_2D_Stub.h>

using namespace LEti;


INIT_FIELDS(LEti::Rigid_Body_2D_Stub, LEti::Object_2D_Stub)
FIELDS_END



void Rigid_Body_2D_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Object_2D_Stub::M_init_constructed_product(_product);

    Object_2D* result = (Object_2D*)_product;

    LPhys::Physics_Module__Rigid_Body_2D* pm = (LPhys::Physics_Module__Rigid_Body_2D*)(result->physics_module());
    pm->set_on_alignment_func([result, pm]()
    {
        result->draw_module()->move_raw(-pm->calculate_raw_center_of_mass());
    });
    pm->align_to_center_of_mass();

    result->set_on_update_func([result](float _ratio)
    {
        LPhys::Physics_Module__Rigid_Body_2D* pm = (LPhys::Physics_Module__Rigid_Body_2D*)result->physics_module();

        result->move(pm->velocity() * LR::Event_Controller::get_dt() * _ratio);
        result->rotate(pm->angular_velocity() * LR::Event_Controller::get_dt() * _ratio);
    });
}
