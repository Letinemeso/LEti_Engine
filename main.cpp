//#include "OpenGL/GLEW/include/glew.h"
//#include "OpenGL/GLFW/include/glfw3.h"
//#include "OpenGL/GLM/mat4x4.hpp"
//
//#include <string>
//#include <iostream>
//#include <chrono>
//
//int main()
//{
//	glfwInit();
//	GLFWwindow* wind = glfwCreateWindow(600, 600, "airstream", 0, 0);
//	glfwMakeContextCurrent(wind);
//	glewInit();
//
//	//shader stuff
//	const char* v_shader_source =
//		"#version 330 core\n"
//		"layout (location = 0) in vec4 pos;\n"
//		"layout (location = 1) in vec4 in_color;\n"
//		"uniform mat4 matrix;\n"
//		"out vec4 v_out_color;\n"
//		"void main()\n"
//		"{\n"
//		"	v_out_color = in_color;\n"
//		"	gl_Position = matrix * pos;\n"
//		"}";
//	const char* f_shader_source =
//		"#version 330 core\n"
//		"in vec4 v_out_color;\n"
//		"out vec4 result_color;\n"
//		"void main()\n"
//		"{\n"
//		"	result_color = v_out_color;\n"
//		"}";
//
//	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//
//	glShaderSource(vertex_shader, 1, &v_shader_source, 0);
//	glShaderSource(fragment_shader, 1, &f_shader_source, 0);
//
//	glCompileShader(vertex_shader);
//	glCompileShader(fragment_shader);
//
//	unsigned int program = glCreateProgram();
//
//	glAttachShader(program, vertex_shader);
//	glAttachShader(program, fragment_shader);
//
//	glLinkProgram(program);
//	glUseProgram(program);
//
//	int link_result = 0;
//	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
//
//	if (link_result == GL_FALSE)
//		std::cout << "no linkie for u\n\n";
//
//	//vertex arrays stuff
//	unsigned int vertex_array;
//	glGenVertexArrays(1, &vertex_array);
//	glBindVertexArray(vertex_array);
//
//	unsigned int buffer[2];
//	glGenBuffers(2, buffer);
//	
//	//vertex buffer
//	float coords[12] =
//	{
//		0.5f, 0.5f, 0.0f, 1.0f,
//		-0.5f, 0.5f, 0.0f, 1.0f,
//		-0.5f, -0.5f, 0.0f, 1.0f
//	};
//
//	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 + 1, coords, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
//
//	//color buffer
//	float colors[12] =
//	{
//		1.0f, 1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f, 1.0f,
//		1.0f, 1.0f, 1.0f, 1.0f
//	};
//
//	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 + 1, colors, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
//
//
//	glm::mat4x4 matrix
//	(
//		1.0f, 0.0f, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f, 0.0f,
//		0.0f, 0.0f, 1.0f, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f
//	);
//	
//	int location = glGetUniformLocation(program, "matrix");
//	if (location == -1) std::cout << "no uniform location for u :D\n\n";
//
//	float delay_between_frames = 1.0f / 60.0f;
//
//	float dt = 0.0f;
//	std::chrono::time_point<std::chrono::steady_clock> time_point_begin;	/* = std::chrono::steady_clock::now()*/
//	std::chrono::time_point<std::chrono::steady_clock> time_point_end;
//	while (!glfwWindowShouldClose(wind))
//	{
//		time_point_begin = std::chrono::steady_clock::now();
//
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		if (dt >= delay_between_frames)
//		{
//			//call update()
//
//			dt = 0.0f;
//		}
//
//		glBindVertexArray(vertex_array);
//		glUniformMatrix4fv(location, 1, false, &matrix[0][0]);
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//
//		glfwPollEvents();
//		glfwSwapBuffers(wind);
//		time_point_end = std::chrono::steady_clock::now();
//
//		std::chrono::duration<float> a(time_point_end - time_point_begin);
//		dt += a.count();
//	}
//
//
//
//
//
//
//
//	glfwDestroyWindow(wind);
//	return 0;
//}
//
//

#include <fstream>

int main()
{
	std::ofstream file(".gitignore", std::ios::trunc);
	file.close();
}
