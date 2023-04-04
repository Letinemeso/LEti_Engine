#include <Object_System/Modules/Draw_Module__Animation.h>

using namespace LEti;


INIT_FIELDS(LEti::Draw_Module__Animation, LEti::Default_Draw_Module_2D)
FIELDS_END


void Draw_Module__Animation::update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 _scale, float _ratio)
{
    Default_Draw_Module_2D::update(_translation, _rotation, _scale, _ratio)
}



void Draw_Module__Animation::set_animation_data(unsigned int _frames_count)
{
    m_frames_count = _frames_count;
    m_frame_offset_ratio = 1.0f / (float)m_frames_count;
}



void Draw_Module__Animation::set_frame(unsigned int _frame)
{
    float current_offset = m_current_frame * m_frame_offset_ratio;
    float needed_offset = _frame * m_frame_offset_ratio;

    for(unsigned int i=0; i<texture().size(); i += 2)
        texture()[i] += needed_offset - current_offset;

    m_current_frame = _frame;
}



INIT_FIELDS(LEti::Draw_Module__Animation__Stub, LEti::Default_Draw_Module_2D_Stub)

ADD_FIELD(unsigned int, frames_per_second)
ADD_FIELD(unsigned int, frames_count)

FIELDS_END



LV::Variable_Base* Draw_Module__Animation__Stub::M_construct_product() const
{
    return new Draw_Module__Animation;
}

void Draw_Module__Animation__Stub::M_init_constructed_product(LV::Variable_Base *_product) const
{
    Draw_Module__Animation* product = (Draw_Module__Animation*)_product;

    product->set_animation_data(frames_count);
    product->set_fps(frames_per_second);
}

