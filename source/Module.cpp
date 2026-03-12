#include <Module.h>

using namespace LEti;


Module::Module()
{

}

Module::~Module()
{

}



void Module::assign_same_parent(Module& _to) const
{
    _to.set_transformation_data(m_transformation_data);
    _to.set_transformation_data_prev_state(m_transformation_data_prev_state);
    _to.set_parent_object(m_parent_object);
}





BUILDER_STUB_CONSTRUCTION_FUNC(Module_Stub)
{
    return nullptr;
}

BUILDER_STUB_INITIALIZATION_FUNC(Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
}
