#include <Object_System/Builder_Stub.h>

using namespace LEti;


INIT_FIELDS(LEti::Builder_Stub, LV::Variable_Base)
FIELDS_END


Builder_Stub::~Builder_Stub()
{

}

LV::Variable_Base* Builder_Stub::construct() const
{
    LV::Variable_Base* result = M_construct_product();
    result->assign_values({});
    M_init_constructed_product(result);
    return result;
}
