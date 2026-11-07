#include <Object.h>

using namespace LEti;


Object::Object()
{
	m_previous_state = m_current_state;
}

Object::~Object()
{
    remove_all_modules();
}



void Object::add_module(Module *_module, bool _push_back)
{
    L_ASSERT(_module);
    L_DEBUG_FUNC_NOARG([&]()
    {
        for(unsigned int i = 0; i < m_modules.size(); ++i)
        {
            L_ASSERT(m_modules[i] != _module);
        }
    });

    _module->set_transformation_data(&m_current_state);
    _module->set_transformation_data_prev_state(&m_previous_state);
    _module->set_parent_object(this);

    if(_push_back)
        m_modules.push(_module);
    else
        m_modules.push_front(_module);
}

void Object::remove_module(Module *_module)
{
    L_ASSERT(_module);

    unsigned int index = 0;
    for(; index < m_modules.size(); ++index)
    {
        if(m_modules[index] == _module)
            break;
    }

    L_ASSERT(index < m_modules.size());

    _module->set_transformation_data(nullptr);
    _module->set_transformation_data_prev_state(nullptr);
    _module->set_parent_object(nullptr);

    m_modules.pop(index);
}

void Object::remove_all_modules()
{
    for(unsigned int i = 0; i < m_modules.size(); ++i)
        delete m_modules[i];

    m_modules.mark_empty();
}



void Object::update_previous_transformation_data()
{
    m_previous_state = m_current_state;
}

void Object::update_previous_state()
{
    m_current_state.mark_unmodified();
    m_previous_state = m_current_state;

    for(unsigned int i = 0; i < m_modules.size(); ++i)
        m_modules[i]->update_prev_state();
}

void Object::update(float _dt)
{
    if(m_on_update_func)
        m_on_update_func(this);

    for(unsigned int i = 0; i < m_modules.size(); ++i)
        m_modules[i]->update(_dt);
}





Object_Stub::~Object_Stub()
{
    clear_childs_list(module_stubs);
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Object_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Object_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->current_state().set_position(position);
    product->current_state().set_scale(scale);
    product->current_state().set_rotation(rotation_angles);

    for(LV::Variable_Base::Childs_List::Const_Iterator it = module_stubs.begin(); !it.end_reached(); ++it)
    {
        Module_Stub* stub = LV::cast_variable<Module_Stub>(it->child_ptr);
        L_ASSERT(stub);

        stub->future_parent = product;
        product->add_module( Module_Stub::construct_from(stub) );
    }
}
