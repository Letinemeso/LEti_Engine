#include <Weak_Module_Holder.h>

using namespace LEti;


Weak_Module_Holder::Weak_Module_Holder()
{

}

Weak_Module_Holder::~Weak_Module_Holder()
{
    if(m_should_delete_module)
        delete m_module;
}



void Weak_Module_Holder::set_module(Module* _ptr)
{
    if(m_module)
    {
        m_module->set_transformation_data(nullptr);
        m_module->set_transformation_data_prev_state(nullptr);
    }

    if(m_should_delete_module)
        delete m_module;

    m_module = _ptr;

    if(!m_module)
        return;

    m_module->set_transformation_data(transformation_data());
    m_module->set_transformation_data_prev_state(transformation_data_prev_state());
}



void Weak_Module_Holder::set_transformation_data(Transformation_Data* _data)
{
    Parent_Type::set_transformation_data(_data);

    if(m_module)
        m_module->set_transformation_data(_data);
}

void Weak_Module_Holder::set_transformation_data_prev_state(const Transformation_Data* _data)
{
    Parent_Type::set_transformation_data_prev_state(_data);

    if(m_module)
        m_module->set_transformation_data_prev_state(_data);
}



void Weak_Module_Holder::update(float _dt)
{
    if(!m_module)
        return;

    m_module->update(_dt);
}





Weak_Module_Holder_Stub::~Weak_Module_Holder_Stub()
{
    delete module_stub;
}



BUILDER_STUB_CONSTRUCTION_FUNC(Weak_Module_Holder_Stub) BUILDER_STUB_CONSTRUCTION_FUNC_DEFAULT_IMPL

BUILDER_STUB_INITIALIZATION_FUNC(Weak_Module_Holder_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;

    if(!module_stub)
        return;

    BUILDER_STUB_CAST_PRODUCT;

    product->set_module((Module*)module_stub->construct());
    product->set_should_delete_draw_module(true);
}
