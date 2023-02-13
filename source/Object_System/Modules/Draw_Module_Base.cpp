#include "../../../include/Object_System/Modules/Draw_Module_Base.h"

using namespace LEti;


void Draw_Module_Base::update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 _scale)
{
    m_transform_matrix = _translation * _rotation * _scale;
}



Draw_Module_Base::Draw_Module_Base()
{

}

Draw_Module_Base::~Draw_Module_Base()
{

}
