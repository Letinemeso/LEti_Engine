#include "../include/Physical_Model_Interface.h"

using namespace LEti;


Physical_Model_Interface::Physical_Model_Interface(const float* _raw_coords, unsigned int _raw_coords_count)
{
    setup(_raw_coords, _raw_coords_count);
}

void Physical_Model_Interface::setup(const float *_raw_coords, unsigned int _raw_coords_count)
{
    delete[] m_raw_coords;

    m_raw_coords_count = _raw_coords_count;
    m_raw_coords = new float[m_raw_coords_count];
    for (unsigned int i = 0; i < m_raw_coords_count; ++i)
        m_raw_coords[i] = _raw_coords[i];
}

Physical_Model_Interface::~Physical_Model_Interface()
{
    delete[] m_raw_coords;
}


void Physical_Model_Interface::set_is_dynamic(bool _is_dynamic)
{
	m_is_dynamic = _is_dynamic;
}

bool Physical_Model_Interface::is_dynamic() const
{
	return m_is_dynamic;
}
