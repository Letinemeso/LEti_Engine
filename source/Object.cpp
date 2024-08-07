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



void Object::add_module(Module *_module)
{
    L_ASSERT(_module);
    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
    {
        if(*it == _module)
        {
            L_ASSERT(false);
            return;
        }
    }

    _module->set_transformation_data(&m_current_state);
    _module->set_transformation_data_prev_state(&m_previous_state);
    _module->set_parent_object(this);

    m_modules.push_back(_module);
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



LV::Variable_Base* Object_Stub::M_construct_product() const
{
    return new Object;
}

void Object_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Object* result = (Object*)_product;

    result->current_state().set_position(position);
    result->current_state().set_scale(scale);
    result->current_state().set_rotation(rotation_angles);

    for(LV::Variable_Base::Childs_List::Const_Iterator it = module_stubs.begin(); !it.end_reached(); ++it)
    {
        Module_Stub* stub = LV::cast_variable<Module_Stub>(it->child_ptr);
        L_ASSERT(stub);
        result->add_module((Module*)stub->construct());
    }
}
