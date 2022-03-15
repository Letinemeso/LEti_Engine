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

	LEti::shader.init_shader("resources\\vertex_shader.shader", "resources\\fragment_shader.shader");
	ASSERT(!LEti::shader());
	LEti::shader.set_matrix_uniform_name("matrix");
	LEti::shader.set_texture_uniform_name("input_texture");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//vertex arrays stuff
	/*unsigned int vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	unsigned int buffer[2];
	glGenBuffers(2, buffer);*/
	
	//vertex buffer
	/*float coords[9] =
	{
		-0.5f, 0.5f,  0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f,  0.5f,  0.0f
	};*/
	float coords[18] =
	{
		-0.5f, 0.5f,  0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f,  0.5f,  0.0f,
		
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	/*LEti::Vertices vertices;
	vertices.load(coords, 18);
	for (unsigned int i = 0; i < 6; ++i)
	{
		for (unsigned int j=0; j<3; ++j)
			std::cout << vertices[i][j] << "\t";
		std::cout << "\n";
	}

	vertices.setup_vertex_buffer(&buffer[0], 0);*/

	//glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	////glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 + 1, coords, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 + 1, coords, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

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

	LEti::Texture texture(texture_name, texture_coords, 12);
	//LEti::Texture texture(texture_name, texture_coords, 6);


	/*glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 + 1, texture.get_tc(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);*/
	//texture.setup_tex_coords_buffer(&buffer[1], 1);


	//ASS EXPERIMENT

	float crds2[9] =
	{
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	unsigned int va2, buf2[2];
	glGenVertexArrays(1, &va2);
	glBindVertexArray(va2);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(2, buf2);

	glBindBuffer(GL_ARRAY_BUFFER, buf2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 + 1, crds2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	/*glBindBuffer(GL_ARRAY_BUFFER, buf2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 + 1, texture.get_tc(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);*/
	texture.setup_tex_coords_buffer(&buf2[1], 1);



	/*int a = 0;
	std::cout << "ready to start search for memory leak\n";
	std::cin >> a;
	while (true)
	{
		LEti::Texture* temp = new LEti::Texture("plug.png", texture_coords, 6);
		delete temp;
		LEti::Shader* shader = new LEti::Shader();
		shader->init_shader("resources\\vertex_shader.shader", "resources\\fragment_shader.shader");
		delete shader;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}*/

	LEti::Object object;
	object.init_texture(texture_name, texture_coords, 12);
	object.init_vertices(coords, 18);

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


		//glBindVertexArray(vertex_array);
		LEti::shader.set_matrix(matrix);

		object.draw();

		/*glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		int location = glGetUniformLocation(LEti::shader.get_program(), "input_texture");
		ASSERT(location == -1);
		glUniform1i(location, 0);*/
		/*LEti::shader.set_texture(texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		glBindVertexArray(va2);
		LEti::shader.set_texture(texture);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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


