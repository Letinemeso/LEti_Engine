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


	//texture stuff
	const char* texture_name = "plug.png";

	//LEti::Texture texture(texture_name, texture_coords, 12);

	//ASS EXPERIMENT

	float crds2[9] =
	{
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	/*unsigned int va2, buf2[2];
	glGenVertexArrays(1, &va2);
	glBindVertexArray(va2);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(2, buf2);

	glBindBuffer(GL_ARRAY_BUFFER, buf2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 + 1, crds2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);*/

	//texture.setup_tex_coords_buffer(&buf2[1], 1);

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
	
	float delay_between_frames = 1.0f / 60.0f;

	float dt = 0.0f;
	std::chrono::time_point<std::chrono::steady_clock> time_point_begin;
	std::chrono::time_point<std::chrono::steady_clock> time_point_end;
	while (!glfwWindowShouldClose(wind))
	{
		time_point_begin = std::chrono::steady_clock::now();

		glClear(GL_COLOR_BUFFER_BIT);


		if (dt >= delay_between_frames)
		{
			//call update()

			dt -= delay_between_frames;
		}

		LEti::Shader::set_matrix(matrix);

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


