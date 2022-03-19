#include "../include/Event_Controller.h"

using namespace LEti;


GLFWwindow* Event_Controller::window = nullptr;


float Event_Controller::desirable_fps = 60.0f;
float Event_Controller::time_before_update = 1.0f / Event_Controller::desirable_fps;
float Event_Controller::dt = 0.0f;

unsigned int Event_Controller::times_to_update = 0;

std::chrono::time_point<std::chrono::steady_clock> Event_Controller::current_time_point = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Event_Controller::prev_time_point = std::chrono::steady_clock::now();

bool Event_Controller::keys_pressed_before[GLFW_KEY_LAST + 1] = { false };



//private methods



//init methods
void Event_Controller::init_and_create_window(unsigned int _width, unsigned int _height, const char* _name)
{
	glfwInit();
	window = glfwCreateWindow(_width, _height, _name, 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();

	ASSERT(window == nullptr);
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

void Event_Controller::process_events()
{
	if (times_to_update == 0) return;

	for (unsigned int i = 0; i <= GLFW_KEY_LAST; ++i)
	{
		if (glfwGetKey(window, i))
		{
			//std::cout << "key pressed: " << i << "\n";
			keys_pressed_before[i] = true;
		}
		else
			keys_pressed_before[i] = false;
	}
}

#include <iostream>

//methods used in game cycle
void Event_Controller::update()
{
	current_time_point = std::chrono::steady_clock::now();
	dt += std::chrono::duration<float, std::ratio<1>>(current_time_point - prev_time_point).count();

	if (dt >= time_before_update)
	{
		while (dt - time_before_update >= 0.0f)
		{
			++times_to_update;
			dt -= time_before_update;
		}
	}
	else times_to_update = 0;

	prev_time_point = current_time_point;

	glfwPollEvents();
}

unsigned int Event_Controller::get_times_to_update()
{
	return times_to_update;
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