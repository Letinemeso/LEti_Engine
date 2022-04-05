#ifndef __EVENT_CONTROLLER
#define __EVENT_CONTROLLER

#include "glew.h"
#include "glfw3.h"

#include "../Debug.h"

#include <chrono>


namespace LEti {

	class Event_Controller
	{
	private:
		struct cursor_position { double x = 0; double y = 0; };
		static cursor_position prev_cursor_pos, current_cursor_pos, cursor_stride;

		struct window_size { unsigned int width = 0.0f, height = 0.0f; };
		static window_size window_data;

	private:
		static GLFWwindow* window;
		
	private:
		static float dt;

		static std::chrono::time_point<std::chrono::steady_clock> current_time_point;
		static std::chrono::time_point<std::chrono::steady_clock> prev_time_point;

	private:
		static bool keys_pressed_before[GLFW_KEY_LAST + 1];

	private:

	public:
		Event_Controller() = delete;
		Event_Controller(const LEti::Event_Controller&) = delete;
		Event_Controller(Event_Controller&&) = delete;

	public:
		static void init_and_create_window(unsigned int _width, unsigned int _height, const char* _name);

	public:
		static bool window_should_close();
		static void swap_buffers();

		static void set_cursor_pos(double _x, double _y);

	public:
		static void update();
		static float get_dt();

		static void update_cursor_stride();

		static bool is_key_down(unsigned int _key);
		static bool is_key_up(unsigned int _key);
		static bool key_was_pressed(unsigned int _key);
		static bool key_was_released(unsigned int _key);

	public:
		static const cursor_position& get_cursor_position();
		static const cursor_position& get_cursor_stride();

		static const window_size& get_window_data();
	};

}

#endif 