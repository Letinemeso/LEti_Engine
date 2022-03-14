#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 in_tex_coords;

uniform mat4 matrix;

out vec2 v_out_tex_coords;

void main()
{
	gl_Position = matrix * vec4(pos, 1.0f);

	v_out_tex_coords = in_tex_coords;
};