#include "../../include/Physics/Narrow_Phase_Interface.h"

using namespace LEti;


void Narrow_Phase_Interface::set_narrowest_phase(Narrowest_Phase_Interface *_narrowest_phase_impl)
{
	delete m_narrowest_phase;
	m_narrowest_phase = _narrowest_phase_impl;
}



Narrow_Phase_Interface::~Narrow_Phase_Interface()
{
	delete m_narrowest_phase;
}
