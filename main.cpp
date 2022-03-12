#include "include/Debug.h"

#include "OpenGL/GLEW/include/glew.h"
#include "OpenGL/GLFW/include/glfw3.h"
#include "OpenGL/GLM/mat4x4.hpp"

#include "OpenGL/stb_image.h"

#include "include/Shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

int main()
{
	ASSERT(!glfwInit());
	GLFWwindow* wind = glfwCreateWindow(600, 600, "airstream", 0, 0);
	glfwMakeContextCurrent(wind);
	ASSERT(glewInit());

	//shader stuff
	/*const char* v_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec4 pos;\n"
		"layout (location = 1) in vec4 in_color;\n"
		"uniform mat4 matrix;\n"
		"out vec4 v_out_color;\n"
		"void main()\n"
		"{\n"
		"	v_out_color = in_color;\n"
		"	gl_Position = matrix * pos;\n"
		"}";
	const char* f_shader_source =
		"#version 330 core\n"
		"in vec4 v_out_color;\n"
		"out vec4 result_color;\n"
		"void main()\n"
		"{\n"
		"	result_color = v_out_color;\n"
		"}";

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader, 1, &v_shader_source, 0);
	glShaderSource(fragment_shader, 1, &f_shader_source, 0);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	unsigned int program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glUseProgram(program);

	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);*/

	LEti::shader.init_shader("resources\\vertex_shader.shader", "resources\\fragment_shader.shader");
	ASSERT(!LEti::shader());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//texture stuff
	const char* texture_name = "plug.png";

	std::ifstream test(texture_name, std::ios::in);
	ASSERT(!test.is_open());
	test.close();

	stbi_set_flip_vertically_on_load(true);
	int size_x, size_y;
	unsigned char* texture_buffer = stbi_load("plug.png", &size_x, &size_y, nullptr, 4);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);
	glActiveTexture(GL_TEXTURE0);

	int location = glGetUniformLocation(LEti::shader.get_program(), "input_texture");
	ASSERT(location == -1);
	glUniform1i(location, 0);

	//vertex arrays stuff
	unsigned int vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	unsigned int buffer[2];
	glGenBuffers(2, buffer);
	
	//vertex buffer
	float coords[12] =
	{
		-0.5f, 0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.0f, 1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 + 1, coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

	//color buffer
	/*float colors[12] =
	{
		0.6f, 0.0f, 1.0f, 1.0f,
		0.6f, 0.0f, 1.0f, 1.0f,
		0.6f, 0.0f, 1.0f, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 + 1, colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);*/

	//texture coordinates stuff
	float texture_coords[6] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 + 1, texture_coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);


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


		glBindVertexArray(vertex_array);
		ASSERT(!LEti::shader.set_uniform(matrix, "matrix"));
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		int location = glGetUniformLocation(LEti::shader.get_program(), "input_texture");
		ASSERT(location == -1);
		glUniform1i(location, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		int error;
		do
		{
			error = glGetError();
			if (error != GL_NO_ERROR) std::cout << error << "\n";
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


