#ifndef __WINDOW_CONTROLLER
#define __WINDOW_CONTROLLER

#include "glew.h"
#include "glfw3.h"

#include "L_Debug/L_Debug.h"

namespace LEti {

class Window_Controller final
{
private:
	static GLFWwindow* m_window;

	struct cursor_position { double x = 0; double y = 0; };
	static cursor_position m_prev_cursor_pos, m_current_cursor_pos, m_cursor_stride;

	struct window_size { unsigned int width = 0.0f, height = 0.0f; };
	static window_size m_window_data;

	static bool m_keys_pressed_before[GLFW_KEY_LAST + 1];
	static bool m_mouse_buttons_pressed_before[GLFW_MOUSE_BUTTON_LAST + 1];

	static int m_mouse_wheel_rotation;

private:
	Window_Controller() = delete;

public:
	static void create_window(unsigned int _width, unsigned int _height, const char* _name);

	static void update();

public:
	static bool window_should_close();
	static void swap_buffers();

	static void set_cursor_pos(double _x, double _y);

	static void update_cursor_stride();

public:
	static const cursor_position& get_cursor_position();
	static const cursor_position& get_cursor_stride();

	static const window_size& get_window_data();

	static bool is_key_down(unsigned int _key);
	static bool is_mouse_button_down(unsigned int _btn);
	static bool key_prev_pressed(unsigned int _key);
	static bool mouse_button_prev_pressed(unsigned int _btn);

	static int mouse_wheel_rotation();

};

}

#endif // __WINDOW_CONTROLLER
