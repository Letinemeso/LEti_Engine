#include <LEti_Registration.h>

#include <Object.h>
#include <Variable_Loader.h>
#include <Weak_Module_Holder.h>
#include <Data_Provider.h>

using namespace LEti;


void LEti::register_types(LV::Object_Constructor& _object_constructor)
{
    _object_constructor.register_type<LEti::Object_Stub>();

    _object_constructor.register_type<LEti::Weak_Module_Holder_Stub>();
}
