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
        for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        {
            if(*it == _module)
            {
                L_ASSERT(false);
                return;
            }
        }
    });

    _module->set_transformation_data(&m_current_state);
    _module->set_transformation_data_prev_state(&m_previous_state);
    _module->set_parent_object(this);

    if(_push_back)
        m_modules.push_back(_module);
    else
        m_modules.push_front(_module);
}

void Object::remove_module(Module *_module)
{
    L_ASSERT(_module);

    LDS::List<Module*>::Iterator it = m_modules.begin();
    for(; !it.end_reached(); ++it)
    {
        if(*it == _module)
            break;
    }

    L_ASSERT(!it.end_reached());
    _module->set_transformation_data(nullptr);
    _module->set_transformation_data_prev_state(nullptr);
    _module->set_parent_object(nullptr);
    if(!it.end_reached())
        m_modules.erase(it);
}

void Object::remove_all_modules()
{
    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        delete *it;

    m_modules.clear();
}



void Object::update_previous_state()
{
    m_current_state.mark_unmodified();
    m_previous_state = m_current_state;

    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        (*it)->update_prev_state();
}

void Object::update(float _dt)
{
    if(m_on_update_func)
        m_on_update_func(this);

    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        (*it)->update(_dt);
}





Object_Stub::~Object_Stub()
{
    for(LV::Variable_Base::Childs_List::Iterator it = module_stubs.begin(); !it.end_reached(); ++it)
        delete it->child_ptr;
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
        product->add_module((Module*)stub->construct());
    }
}
