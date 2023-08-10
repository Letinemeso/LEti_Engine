#include <Object_System/Object_2D.h>

using namespace LEti;


INIT_FIELDS(LEti::Object_2D_Stub, LV::Builder_Stub)

ADD_FIELD(glm::vec3, position)
ADD_FIELD(glm::vec3, scale)
ADD_FIELD(glm::vec3, rotation_axis)
ADD_FIELD(float, rotation_angle)

ADD_FIELD(bool, enable_draw_module)
ADD_CHILD("draw_module", *draw_module)

FIELDS_END



Object_2D_Stub::~Object_2D_Stub()
{
    delete draw_module;
}



LV::Variable_Base* Object_2D_Stub::M_construct_product() const
{
    return new Object_2D;
}

void Object_2D_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Object_2D* result = (Object_2D*)_product;

    result->set_pos(position);
    result->set_scale(scale);
    result->set_rotation_axis(rotation_axis);
    result->set_rotation_angle(rotation_angle);

    if(enable_draw_module)
        result->set_draw_module((LR::Default_Draw_Module_2D*)draw_module->construct());
}


INIT_FIELDS(LEti::Object_2D, LEti::Object_Base)
FIELDS_END



Object_2D::Object_2D() : Object_Base()
{
	m_previous_state = m_current_state;
}

Object_2D::~Object_2D()
{
    remove_draw_module();

    remove_all_modules();
}



void Object_2D::set_pos(const glm::vec3 &_position)
{
    m_current_state.set_position(_position);
}

void Object_2D::move(const glm::vec3 &_stride)
{
    m_current_state.move(_stride);
}


void Object_2D::set_rotation_axis(const glm::vec3 &_axis)
{

}

void Object_2D::set_rotation_angle(float _angle)
{
    m_current_state.set_rotation({0.0f, 0.0f, _angle});
}

void Object_2D::rotate(float _angle)
{
    m_current_state.rotate({0.0f, 0.0f, _angle});
}


void Object_2D::set_scale(const glm::vec3 &_scale)
{
    m_current_state.set_scale(_scale);
}

void Object_2D::set_scale(float _scale)
{
    m_current_state.set_scale({_scale, _scale, _scale});
}



glm::vec3 Object_2D::get_pos() const
{
    return m_current_state.position();
}

glm::vec3 Object_2D::get_scale() const
{
    return m_current_state.scale();
}

glm::vec3 Object_2D::get_rotation_axis() const      //  TODO: remove outdated stuff
{
    return {0.0f, 0.0f, 1.0f};
}

float Object_2D::get_rotation_angle() const
{
    return m_current_state.rotation().z;
}


glm::vec3 Object_2D::get_pos_prev() const
{
    return m_previous_state.position();
}

glm::vec3 Object_2D::get_scale_prev() const
{
    return m_previous_state.scale();
}

glm::vec3 Object_2D::get_rotation_axis_prev() const
{
    return {0.0f, 0.0f, 1.0f};
}

float Object_2D::get_rotation_angle_prev() const
{
    return m_previous_state.rotation().z;
}



void Object_2D::add_module(Module *_module)
{
    L_ASSERT(_module);
    for(LDS::List<Module*>::Iterator it = m_modules.begin(); !it.end_reached(); ++it)
    {
        L_ASSERT(*it != _module);
        return;
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


void Object_2D::set_draw_module(LR::Default_Draw_Module_2D* _module)
{
    delete m_draw_module;
    m_draw_module = _module;
}

void Object_2D::remove_draw_module()
{
	delete m_draw_module;
	m_draw_module = nullptr;
}

LR::Default_Draw_Module_2D* Object_2D::draw_module()
{
	return m_draw_module;
}

const LR::Default_Draw_Module_2D* Object_2D::draw_module() const
{
	return m_draw_module;
}



void Object_2D::revert_to_previous_state()
{
	m_current_state = m_previous_state;
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

    if(m_draw_module)
        m_draw_module->update(matrix);
}

void Object_2D::draw(const LR::Renderer &_renderer) const
{
    _renderer.draw(*draw_module());
}
