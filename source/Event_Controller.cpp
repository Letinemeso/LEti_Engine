#include "../include/Event_Controller.h"

using namespace LEti;

float Event_Controller::m_dt = 0.0f;
float Event_Controller::m_max_dt = -1.0f;
std::chrono::time_point<std::chrono::steady_clock> Event_Controller::m_current_time_point = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Event_Controller::m_prev_time_point = std::chrono::steady_clock::now();


void Event_Controller::set_max_dt(float _max_dt)
{
	m_max_dt = _max_dt;
}



void Event_Controller::update()
{
	m_current_time_point = std::chrono::steady_clock::now();
	m_dt = std::chrono::duration<float, std::ratio<1>>(m_current_time_point - m_prev_time_point).count();
	if(m_max_dt > 0.0f)
		if(m_dt > m_max_dt)
			m_dt = m_max_dt;

	m_prev_time_point = m_current_time_point;
}

float Event_Controller::get_dt()
{
	return m_dt;
}


bool Event_Controller::is_mouse_button_down(unsigned int _btn)
{
	return Window_Controller::is_mouse_button_down(_btn);
}

bool Event_Controller::is_mouse_button_up(unsigned int _btn)
{
	return !Window_Controller::is_mouse_button_down(_btn);
}

bool Event_Controller::mouse_button_was_pressed(unsigned int _btn)
{
	return Window_Controller::is_mouse_button_down(_btn) && !Window_Controller::mouse_button_prev_pressed(_btn);
}

bool Event_Controller::mouse_button_was_released(unsigned int _btn)
{
	return !Window_Controller::is_mouse_button_down(_btn) && Window_Controller::mouse_button_prev_pressed(_btn);
}

int Event_Controller::mouse_wheel_rotation()
{
	return Window_Controller::mouse_wheel_rotation();
}


bool Event_Controller::is_key_down(unsigned int _key)
{
	return Window_Controller::is_key_down(_key);
}

bool Event_Controller::is_key_up(unsigned int _key)
{
	return !Window_Controller::is_key_down(_key);
}

bool Event_Controller::key_was_pressed(unsigned int _key)
{
	return Window_Controller::is_key_down(_key) && !Window_Controller::key_prev_pressed(_key);
}

bool Event_Controller::key_was_released(unsigned int _key)
{
	return !Window_Controller::is_key_down(_key) && Window_Controller::key_prev_pressed(_key);
}
