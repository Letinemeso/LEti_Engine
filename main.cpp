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

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <thread>

using LEC = LEti::Event_Controller;

int main()
{
	LEti::Event_Controller::init_and_create_window(1422, 800, "engine-test");

	LEti::Shader::init_shader("resources\\vertex_shader.shader", "resources\\fragment_shader.shader");
	ASSERT(!LEti::Shader::is_valid());
	LEti::Shader::set_projection_matrix_uniform_name("projection_matrix");
	LEti::Shader::set_transform_matrix_uniform_name("transform_matrix");
	LEti::Shader::set_texture_uniform_name("input_texture");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	glm::mat4x4 orthographic_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	//LEti::Shader::set_projection_matrix(orthographic_matrix);

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

		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	const char* texture_name = "plug.png";

	float crds2[9] =
	{
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	float texture_coords_ymany[6] =
	{
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	LEti::Object object;
	object.init_texture(texture_name, texture_coords, 12);
	object.init_vertices(coords, 18);

	LEti::Object object2;
	object2.init_texture("ymany.png", texture_coords_ymany, 6);
	object2.init_vertices(crds2, 9);





	//matrix stuff

	float camera_rotation_angle = 0.0f;

	glm::vec3 point_of_view(sin(camera_rotation_angle), 0.0f, cos(camera_rotation_angle));
	glm::vec3 camera_position(0.0f, 0.0f, -1.0f);
	glm::vec3 camera_top(0.0f, 1.0f, 0.0f);
	glm::mat4x4 look_at_matrix = glm::lookAt(camera_position, point_of_view + camera_position, camera_top);

	glm::mat4x4 perspective_matrix = glm::perspective<float>(3.14159 / 2.0f, 1422.0f/800.0f, 0.001f, 10.0f);

	glm::mat4x4 camera_matrix = perspective_matrix * look_at_matrix;







	//some shit, idk

	float ortho_value = 2.0f;

	unsigned int current_rotation_state = 0;

	float scale = 1.0f;
	
	object.set_pos(0.0f, 0.0f, -0.1f);
	object.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);

	object2.set_pos(0.0f, 0.5f, 0.0f);

	object2.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);
	object2.set_overall_scale(1.0f);

	while (!LEti::Event_Controller::window_should_close())
	{
		LEti::Event_Controller::update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(unsigned int i=0; i<LEti::Event_Controller::get_times_to_update(); ++i)
		{
			//call update()
			/*if (LEti::Event_Controller::is_key_down(GLFW_KEY_UP))
			{
				(scale + 0.01f >= 1.0f ? scale = 1.0f : scale += 0.01f);
			}
			if (LEti::Event_Controller::is_key_down(GLFW_KEY_DOWN))
			{
				(scale - 0.01f <= 0.2f ? scale = 0.2f : scale -= 0.01f);
			}*/
			constexpr float movement_speed = 0.5f / 60.0f;

			constexpr float half_str_angle = 6.28318f / 8.0f;

			bool is_moving = false;
			float movement_angle = camera_rotation_angle;

			if (LEC::is_key_down(GLFW_KEY_W))
			{
				if (LEC::is_key_down(GLFW_KEY_A))
					movement_angle += half_str_angle;
				else if (LEC::is_key_down(GLFW_KEY_D))
					movement_angle -= half_str_angle;
				is_moving = true;
			}
			else if (LEC::is_key_down(GLFW_KEY_S))
			{
				movement_angle += half_str_angle * 4.0f;
				if (LEC::is_key_down(GLFW_KEY_A))
					movement_angle -= half_str_angle;
				else if (LEC::is_key_down(GLFW_KEY_D))
					movement_angle += half_str_angle;
				is_moving = true;
			}
			else if (LEC::is_key_down(GLFW_KEY_A))
			{
				movement_angle += half_str_angle * 2.0f;
				is_moving = true;
			}
			else if (LEC::is_key_down(GLFW_KEY_D))
			{
				movement_angle -= half_str_angle * 2.0f;
				is_moving = true;
			}

			if (is_moving)
			{
				camera_position[0] += movement_speed * sin(movement_angle);
				camera_position[2] += movement_speed * cos(movement_angle);
			}
			/*if (LEC::is_key_down(GLFW_KEY_W))
			{
				camera_position[0] += movement_speed * sin(camera_rotation_angle);
				camera_position[2] += movement_speed * cos(camera_rotation_angle);
			}
			if (LEC::is_key_down(GLFW_KEY_S))
			{
				camera_position[0] -= movement_speed * sin(camera_rotation_angle);
				camera_position[2] -= movement_speed * cos(camera_rotation_angle);
			}
			if (LEC::is_key_down(GLFW_KEY_A))
			{
				camera_position[0] += movement_speed * cos(camera_rotation_angle);
				camera_position[2] -= movement_speed * sin(camera_rotation_angle);
			}
			if (LEC::is_key_down(GLFW_KEY_D))
			{
				camera_position[0] -= movement_speed * cos(camera_rotation_angle);
				camera_position[2] += movement_speed * sin(camera_rotation_angle);
			}*/

			if (LEC::is_key_down(GLFW_KEY_LEFT))
			{
				camera_rotation_angle += 6.28318f / 4.0f / 60.0f;
			}
			if (LEC::is_key_down(GLFW_KEY_RIGHT))
			{
				camera_rotation_angle -= 6.28318f / 4.0f / 60.0f;
			}
			

			while (camera_rotation_angle > 6.28318f)
				camera_rotation_angle -= 6.28318f;
			while (camera_rotation_angle < 0.0f)
				camera_rotation_angle += 6.28318f;


			point_of_view = glm::vec3(sin(camera_rotation_angle), 0.0f, cos(camera_rotation_angle));

			look_at_matrix = glm::lookAt(camera_position, point_of_view + camera_position, camera_top);

			camera_matrix = perspective_matrix * look_at_matrix;

			LEti::Shader::set_projection_matrix(camera_matrix);

			object.set_overall_scale(scale);
			object2.set_overall_scale(scale);
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


