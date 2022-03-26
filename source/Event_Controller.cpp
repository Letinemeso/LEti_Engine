#include "../include/Event_Controller.h"

using namespace LEti;


GLFWwindow* Event_Controller::window = nullptr;


//float Event_Controller::tickrate = 60.0f;
//float Event_Controller::time_before_update = 1.0f / Event_Controller::tickrate;
float Event_Controller::dt = 0.0f;

//unsigned int Event_Controller::times_to_update = 0;

std::chrono::time_point<std::chrono::steady_clock> Event_Controller::current_time_point = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Event_Controller::prev_time_point = std::chrono::steady_clock::now();

bool Event_Controller::keys_pressed_before[GLFW_KEY_LAST + 1] = { false };

Event_Controller::cursor_position Event_Controller::prev_cursor_pos,
								  Event_Controller::current_cursor_pos,
								  Event_Controller::cursor_stride;

Event_Controller::window_size Event_Controller::window_data;



//private methods



//init methods
void Event_Controller::init_and_create_window(unsigned int _width, unsigned int _height, const char* _name, float _tickrate)
{
	glfwInit();
	window = glfwCreateWindow(_width, _height, _name, 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();

	ASSERT(window == nullptr);

	window_data.width = _width;
	window_data.height = _height;
}



//utility methods
bool Event_Controller::window_should_close()
{
	return glfwWindowShouldClose(window);
}

void Event_Controller::swap_buffers()
{
	glfwSwapBuffers(window);
}

void Event_Controller::set_cursor_pos(double _x, double _y)
{
	glfwSetCursorPos(window, _x, _y);

	cursor_stride.x = 0.0;
	cursor_stride.y = 0.0;
	prev_cursor_pos.x = _x;
	prev_cursor_pos.y = _y;
}

//methods used in game cycle
void Event_Controller::update()
{
	current_time_point = std::chrono::steady_clock::now();
	dt = std::chrono::duration<float, std::ratio<1>>(current_time_point - prev_time_point).count();

	prev_time_point = current_time_point;

	for (unsigned int i = 0; i <= GLFW_KEY_LAST; ++i)
		keys_pressed_before[i] = glfwGetKey(window, i);
	glfwPollEvents();

	glfwGetCursorPos(window, &current_cursor_pos.x, &current_cursor_pos.y);
}

float Event_Controller::get_dt()
{
	return dt;
}


void Event_Controller::update_cursor_stride()
{
	cursor_stride.x = prev_cursor_pos.x - current_cursor_pos.x;
	cursor_stride.y = prev_cursor_pos.y - current_cursor_pos.y;
	prev_cursor_pos.x = current_cursor_pos.x;
	prev_cursor_pos.y = current_cursor_pos.y;
}


bool Event_Controller::is_key_down(unsigned int _key)
{
	return glfwGetKey(window, _key) == true;
}

bool Event_Controller::is_key_up(unsigned int _key)
{
	return glfwGetKey(window, _key) == false;
}

bool Event_Controller::key_was_pressed(unsigned int _key)
{
	return is_key_down(_key) && !keys_pressed_before[_key];
}

bool Event_Controller::key_was_released(unsigned int _key)
{
	return !is_key_down(_key) && keys_pressed_before[_key];
}



const Event_Controller::cursor_position& Event_Controller::get_cursor_position()
{
	return current_cursor_pos;
}

const Event_Controller::cursor_position& Event_Controller::get_cursor_stride()
{
	return cursor_stride;
}


const Event_Controller::window_size& Event_Controller::get_window_data()
{
	return window_data;
}
