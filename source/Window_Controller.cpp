#include "../include/Window_Controller.h"

using namespace LEti;

GLFWwindow* Window_Controller::m_window = nullptr;
Window_Controller::cursor_position Window_Controller::m_prev_cursor_pos, Window_Controller::m_current_cursor_pos, Window_Controller::m_cursor_stride;
Window_Controller::window_size Window_Controller::m_window_data;
bool Window_Controller::m_keys_pressed_before[GLFW_KEY_LAST + 1] = { false };
bool Window_Controller::m_mouse_buttons_pressed_before[GLFW_MOUSE_BUTTON_LAST + 1] = { false };


void Window_Controller::create_window(unsigned int _width, unsigned int _height, const char* _name)
{
	glfwInit();
	m_window = glfwCreateWindow(_width, _height, _name, 0, 0);
	glfwMakeContextCurrent(m_window);
	glewInit();

	glfwSetWindowSizeLimits(m_window, _width, _height, _width, _height);

	ASSERT(m_window == nullptr);

	m_window_data.width = _width;
	m_window_data.height = _height;
}


void Window_Controller::update()
{
	for (unsigned int i = 0; i <= GLFW_KEY_LAST; ++i)
		m_keys_pressed_before[i] = glfwGetKey(m_window, i);
	for (unsigned int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
		m_mouse_buttons_pressed_before[i] = glfwGetMouseButton(m_window, i);
	glfwPollEvents();

	glfwGetCursorPos(m_window, &m_current_cursor_pos.x, &m_current_cursor_pos.y);
	m_current_cursor_pos.y = m_window_data.height - m_current_cursor_pos.y;
}



bool Window_Controller::window_should_close()
{
	return glfwWindowShouldClose(m_window);
}

void Window_Controller::swap_buffers()
{
	glfwSwapBuffers(m_window);
}


void Window_Controller::set_cursor_pos(double _x, double _y)
{
	glfwSetCursorPos(m_window, _x, _y);

	m_cursor_stride.x = 0.0;
	m_cursor_stride.y = 0.0;
	m_prev_cursor_pos.x = _x;
	m_prev_cursor_pos.y = _y;
}


void Window_Controller::update_cursor_stride()
{
	m_cursor_stride.x = m_prev_cursor_pos.x - m_current_cursor_pos.x;
	m_cursor_stride.y = m_prev_cursor_pos.y - m_current_cursor_pos.y;
	m_prev_cursor_pos.x = m_current_cursor_pos.x;
	m_prev_cursor_pos.y = m_current_cursor_pos.y;
}



const Window_Controller::cursor_position& Window_Controller::get_cursor_position()
{
	return m_current_cursor_pos;
}

const Window_Controller::cursor_position& Window_Controller::get_cursor_stride()
{
	return m_cursor_stride;
}


const Window_Controller::window_size& Window_Controller::get_window_data()
{
	return m_window_data;
}


bool Window_Controller::is_key_down(unsigned int _key)
{
	return glfwGetKey(m_window, _key) == true;
}

bool Window_Controller::is_mouse_button_down(unsigned int _btn)
{
	return glfwGetMouseButton(m_window, _btn) == true;
}

bool Window_Controller::key_prev_pressed(unsigned int _key)
{
	return m_keys_pressed_before[_key];
}

bool Window_Controller::mouse_button_prev_pressed(unsigned int _btn)
{
	return m_mouse_buttons_pressed_before[_btn];
}

