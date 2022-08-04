#include "../include/Event_Controller.h"

using namespace LEti;

float Event_Controller::dt = 0.0f;
std::chrono::time_point<std::chrono::steady_clock> Event_Controller::current_time_point = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Event_Controller::prev_time_point = std::chrono::steady_clock::now();



//methods used in game cycle
void Event_Controller::update()
{
	current_time_point = std::chrono::steady_clock::now();
	dt = std::chrono::duration<float, std::ratio<1>>(current_time_point - prev_time_point).count();

	prev_time_point = current_time_point;
}

float Event_Controller::get_dt()
{
	return dt;
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
