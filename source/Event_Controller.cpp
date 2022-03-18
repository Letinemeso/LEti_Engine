#include "../include/Event_Controller.h"

using namespace LEti;


GLFWwindow* Event_Controller::window = nullptr;


void Event_Controller::init_and_create_window(unsigned int _width, unsigned int _height, const char* _name)
{
	glfwInit();
	window = glfwCreateWindow(_width, _height, _name, 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}


