#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 in_color;

uniform mat4 matrix;

out vec4 v_out_color;

void main()
{
	v_out_color = in_color;
	gl_Position = matrix * pos;
};