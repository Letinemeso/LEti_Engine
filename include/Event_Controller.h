#ifndef __EVENT_CONTROLLER
#define __EVENT_CONTROLLER

#include "glew.h"
#include "glfw3.h"

#include "../Debug.h"

#include "../include/Window_Controller.h"

#include <chrono>


namespace LEti {

	class Event_Controller final
	{
	private:
		static float dt;

		static std::chrono::time_point<std::chrono::steady_clock> current_time_point;
		static std::chrono::time_point<std::chrono::steady_clock> prev_time_point;

	private:
		Event_Controller() = delete;
		Event_Controller(const LEti::Event_Controller&) = delete;
		Event_Controller(Event_Controller&&) = delete;

	public:
		static void update();
		static float get_dt();

        static bool is_mouse_button_down(unsigned int _btn);
        static bool is_mouse_button_up(unsigned int _btn);
        static bool mouse_button_was_pressed(unsigned int _btn);
        static bool mouse_button_was_released(unsigned int _btn);

		static bool is_key_down(unsigned int _key);
		static bool is_key_up(unsigned int _key);
		static bool key_was_pressed(unsigned int _key);
		static bool key_was_released(unsigned int _key);

	};

}

#endif 
