#include <Object_System/Object_2D.h>

using namespace LEti;


INIT_FIELDS(LEti::Object_2D_Stub, LV::Builder_Stub)

ADD_FIELD(glm::vec3, position)
ADD_FIELD(glm::vec3, scale)
ADD_FIELD(glm::vec3, rotation_axis)
ADD_FIELD(float, rotation_angle)

//ADD_FIELD(bool, enable_draw_module)
//ADD_CHILD("draw_module", *draw_module)

FIELDS_END



Object_2D_Stub::~Object_2D_Stub()
{
//    delete draw_module;
}



LV::Variable_Base* Object_2D_Stub::M_construct_product() const
{
    return new Object_2D;
}

void Object_2D_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Object_2D* result = (Object_2D*)_product;

    result->current_state().set_position(position);
    result->current_state().set_scale(scale);
    result->current_state().set_rotation({0.0f, 0.0f, rotation_angle});

}


INIT_FIELDS(LEti::Object_2D, LEti::Object_Base)
FIELDS_END



Object_2D::Object_2D() : Object_Base()
{
	m_previous_state = m_current_state;
}

Object_2D::~Object_2D()
{
    remove_all_modules();
}



void Object_2D::add_module(Module *_module)
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
    m_modules.push_back(_module);
}

void Object_2D::remove_module(Module *_module)
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
    if(!it.end_reached())
        m_modules.erase(it);
}

void Object_2D::remove_all_modules()
{
    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        delete *it;

    m_modules.clear();
}



void Object_2D::update_previous_state()
{
    m_previous_state = m_current_state;

    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        (*it)->update_prev_state();
}

void Object_2D::update()
{
    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
        (*it)->update();

    m_current_state.update_matrix();
    glm::mat4x4 matrix = m_current_state.matrix();
}
