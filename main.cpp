#include "Debug.h"

#include "OpenGL/GLEW/include/glew.h"
#include "OpenGL/GLFW/include/glfw3.h"
#include "OpenGL/GLM/mat4x4.hpp"

#include "include/Shader.h"
#include "include/Texture.h"
#include "include/Object.h"
#include "include/Vertices.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <thread>

int main()
{
	bool glfw_init_result = glfwInit();
	GLFWwindow* wind = glfwCreateWindow(600, 600, "airstream", 0, 0);
	glfwMakeContextCurrent(wind);
	bool glew_init_result = glewInit();	//returns false for some reason
	//ASSERT(!glfw_init_result || !glew_init_result);

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



	float scale = 1.0f;
	
	object.set_pos(0.0f, 0.0f, -0.1f);
	object.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);
	object2.set_rotation_data(0.0f, 0.0f, 1.0f, 0.0f);
	object2.set_overall_scale(1.0f);

	float delay_between_frames = 1.0f / 60.0f;
	float dt = 0.0f;
	std::chrono::time_point<std::chrono::steady_clock> time_point_begin;
	std::chrono::time_point<std::chrono::steady_clock> time_point_end;
	while (!glfwWindowShouldClose(wind))
	{
		time_point_begin = std::chrono::steady_clock::now();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (dt >= delay_between_frames)
		{
			//call update()
			if (glfwGetKey(wind, GLFW_KEY_UP) == GLFW_PRESS)
			{
				//object.move(0.0f, 0.0f, -0.05f);
				(scale + 0.01f >= 1.0f ? scale = 1.0f : scale += 0.01f);
			}
			if (glfwGetKey(wind, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				//object.move(0.0f, 0.0f, 0.05f);
				(scale - 0.01f <= 0.2f ? scale = 0.2f : scale -= 0.01f);
			}

			object.set_overall_scale(scale);
			object2.set_overall_scale(scale);

			object.rotate(6.28318f / (60.0f * 4.0f));
			object2.rotate(-6.28318f / (60.0f * 4.0f));

			dt -= delay_between_frames;
		}

		//LEti::Shader::set_matrix(matrix);

		object.draw();
		object2.draw();

		int error;
		do
		{
			error = glGetError();
			if (error != GL_NO_ERROR) std::cout << "error: " << error << "\n";
		} while (error != GL_NO_ERROR);

		glfwPollEvents();
		glfwSwapBuffers(wind);
		time_point_end = std::chrono::steady_clock::now();

		std::chrono::duration<float, std::ratio<1>> a(time_point_end - time_point_begin);
		dt += a.count();
	}







	glfwDestroyWindow(wind);
	return 0;
}


