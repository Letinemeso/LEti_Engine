#include "../../include/Physics/Narrow_Phase_Interface.h"

using namespace LEti;


Narrow_Phase_Interface::~Narrow_Phase_Interface()
{
	delete m_narrowest_phase;
}
