#include "../include/Timer.h"

using namespace LEti;


void Timer::start(float _alarm_time)
{
	m_alarm_time = _alarm_time;
	m_current_time = 0.0f;
	m_active = true;
}

void Timer::reset()
{
	m_current_time = 0.0f;
	m_active = false;
}


void Timer::update()
{
    if(!m_active)
        return;

	m_current_time += LEti::Event_Controller::get_dt();
    if(m_current_time >= m_alarm_time)
        reset();
}
