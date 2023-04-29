#include "../../../include/Object_System/Modules/Draw_Module_Base.h"

using namespace LEti;


INIT_FIELDS(LEti::Draw_Module_Base_Stub, LEti::Builder_Stub)
FIELDS_END


INIT_FIELDS(LEti::Draw_Module_Base, LV::Variable_Base)
FIELDS_END



void Draw_Module_Base::update(const glm::mat4x4& _matrix)
{
    m_transform_matrix = _matrix;
}



Draw_Module_Base::Draw_Module_Base()
{

}

Draw_Module_Base::~Draw_Module_Base()
{

}
