#include "Debug.h"

#include "OpenGL/GLEW/include/glew.h"
#include "OpenGL/GLFW/include/glfw3.h"
#include "OpenGL/GLM/mat4x4.hpp"

#include "include/Shader.h"
#include "include/Texture.h"
#include "include/Object.h"
#include "include/Vertices.h"
#include "include/Event_Controller.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <thread>

using LEC = LEti::Event_Controller;

int main()
{
	LEti::Event_Controller::init_and_create_window(600, 600, "engine-test");

	LEti::Shader::init_shader("resources\\vertex_shader.shader", "resources\\fragment_shader.shader");
	ASSERT(!LEti::Shader::is_valid());
	LEti::Shader::set_matrix_uniform_name("matrix");
	LEti::Shader::set_texture_uniform_name("input_texture");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	//vertex buffer
	float coords[18] =
	{
		-0.5f, 0.5f,  0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f,  0.5f,  0.0f,
		
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	float texture_coords[12] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	const char* texture_name = "plug.png";

	float crds2[9] =
	{
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	LEti::Object object;
	object.init_texture(texture_name, texture_coords, 12);
	object.init_vertices(coords, 18);

	LEti::Object object2;
	object2.init_texture(texture_name, texture_coords, 6);
	object2.init_vertices(crds2, 9);

	//matrix stuff
	glm::mat4x4 matrix
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	unsigned int current_rotation_state = 0;

	float scale = 1.0f;
	
	object.set_pos(0.0f, 0.0f, -0.1f);
	object.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);
	object2.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);
	object2.set_overall_scale(1.0f);

	while (!LEti::Event_Controller::window_should_close())
	{
		LEti::Event_Controller::update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(unsigned int i=0; i<LEti::Event_Controller::get_times_to_update(); ++i)
		{
			//call update()
			if (LEti::Event_Controller::is_key_down(GLFW_KEY_UP))
			{
				//object.move(0.0f, 0.0f, -0.05f);
				(scale + 0.01f >= 1.0f ? scale = 1.0f : scale += 0.01f);
			}
			if (LEti::Event_Controller::is_key_down(GLFW_KEY_DOWN))
			{
				//object.move(0.0f, 0.0f, 0.05f);
				(scale - 0.01f <= 0.2f ? scale = 0.2f : scale -= 0.01f);
			}
			if (LEC::key_was_pressed(GLFW_KEY_LEFT))
			{
				current_rotation_state = (current_rotation_state == 0 ? 3 : current_rotation_state - 1);
				object.set_rotation_angle(6.28318f / 4.0f * current_rotation_state);
				object2.set_rotation_angle(6.28318f / 4.0f * current_rotation_state);
			}
			if (LEC::key_was_pressed(GLFW_KEY_RIGHT))
			{
				current_rotation_state = (current_rotation_state == 3 ? 0 : current_rotation_state + 1);
				object.set_rotation_angle(6.28318f / 4.0f * current_rotation_state);
				object2.set_rotation_angle(6.28318f / 4.0f * current_rotation_state);
			}

			object.set_overall_scale(scale);
			object2.set_overall_scale(scale);

			/*object.rotate(6.28318f / (60.0f * 4.0f));
			object2.rotate(-6.28318f / (60.0f * 4.0f));*/
		}

		object.draw();
		object2.draw();

		int error;
		do
		{
			error = glGetError();
			if (error != GL_NO_ERROR) std::cout << "error: " << error << "\n";
		} while (error != GL_NO_ERROR);

		LEti::Event_Controller::process_events();
		LEti::Event_Controller::swap_buffers();
	}







	//glfwDestroyWindow(wind);
	return 0;
}


