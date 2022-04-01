#include "Debug.h"

#include "OpenGL/GLEW/include/glew.h"
#include "OpenGL/GLFW/include/glfw3.h"
#include "OpenGL/GLM/mat4x4.hpp"
#include "OpenGL/GLM/gtx/transform.hpp"

#include "include/Shader.h"
#include "include/Texture.h"
#include "include/Object.h"
#include "include/Vertices.h"
#include "include/Event_Controller.h"
#include "include/Camera.h"
#include "include/Resource_Loader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <thread>

using LEC = LEti::Event_Controller;

int main()
{
	LEti::Event_Controller::init_and_create_window(1422, 800, "LEti");

	LEti::Shader::init_shader("resources/vertex_shader.shader", "resources/fragment_shader.shader");
	ASSERT(!LEti::Shader::is_valid());
	LEti::Shader::set_projection_matrix_uniform_name("projection_matrix");
	LEti::Shader::set_transform_matrix_uniform_name("transform_matrix");
	LEti::Shader::set_texture_uniform_name("input_texture");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	//vertex buffer
	LEti::Resource_Loader::load_object("quad", "resources/models/quad.mdl");

	float crds2[9] =
	{
		1.0f, 100.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		100.0f, 1.0f, 0.0f
	};

	float texture_coords_ymany[6] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	glm::vec3 v(0.0f, 0.0f, 0.0f);
	float a = LEti::Utility::vector_length(v);
	LEti::Utility::shrink_vector_to_1(v);

	LEti::Object object;
	auto tcoords = LEti::Resource_Loader::get_data<float>("quad", "texture_coords");
	object.init_texture(LEti::Resource_Loader::get_data<std::string>("quad", "texture_name").first->c_str(), tcoords.first, tcoords.second);

	auto coords = LEti::Resource_Loader::get_data<float>("quad", "coords");
	object.init_vertices(coords.first, coords.second);

	LEti::Object object2(false);
	object2.init_texture("ymany.png", texture_coords_ymany, 6);
	object2.init_vertices(crds2, 9);

	//matrix to rotate camera_top vector

	LEti::Camera::set_fov_and_max_distance(3.14159 / 1.8f, 5.0f);
	LEti::Camera::set_camera_data({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f });
	LEti::Camera::setup_orthographic_matrix();

	//some shit, idk

	float ortho_value = 2.0f;

	float scale = 1.0f;

	object.set_pos(0.0f, 0.0f, -0.1f);
	object.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);

	object2.set_pos(0.0f, 0.5f, 0.0f);

	object2.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);
	object2.set_overall_scale(1.0f);

	float time_has_passed = 0.0f;
	unsigned int fps = 0;

	while (!LEti::Event_Controller::window_should_close())
	{
		LEti::Event_Controller::update();

		time_has_passed += LEti::Event_Controller::get_dt();
		++fps;
		if (time_has_passed > 1.0f)
		{
			std::cout << "fps: " << fps << "\n";
			time_has_passed -= 1.0f;
			fps = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (LEti::Event_Controller::key_was_released(GLFW_KEY_TAB))
			LEti::Camera::toggle_controll(LEti::Camera::get_controllable() == true ? false : true);
		LEti::Camera::update(true, true);

		object.draw();
		object2.draw();





		LEti::Event_Controller::swap_buffers();

		int error;
		do
		{
			error = glGetError();
			if (error != GL_NO_ERROR)
			{
				std::cout << "error: " << error << "\n";
				std::cout << glewGetErrorString(error) << "\n\n";
				std::this_thread::sleep_for(std::chrono::seconds(10));
			}
		} while (error != GL_NO_ERROR);

		//std::this_thread::sleep_for(std::chrono::microseconds(1));
	}







	//glfwDestroyWindow(wind);
	return 0;
}


