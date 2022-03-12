#version 330 core

layout(location = 0) in vec4 pos;
//layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_tex_coords;

uniform mat4 matrix;

//out vec4 v_out_color;
out vec2 v_out_tex_coords;

void main()
{
	gl_Position = matrix * pos;
	//v_out_color = in_color;
	v_out_tex_coords = in_tex_coords;
};