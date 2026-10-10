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
