#ifndef __EVENT_CONTROLLER
#define __EVENT_CONTROLLER

#include "../OpenGL/GLEW/include/glew.h"
#include "../OpenGL/GLFW/include/glfw3.h"


namespace LEti {

	class Event_Controller
	{
	private:
		static GLFWwindow* window;

	public:
		Event_Controller() = delete;
		Event_Controller(const LEti::Event_Controller&) = delete;
		Event_Controller(Event_Controller&&) = delete;

	public:
		static void init_and_create_window(unsigned int _width, unsigned int _height, const char* _name);

	public:


	};

}

#endif 