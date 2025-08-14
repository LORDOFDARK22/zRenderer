#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 o_Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * vec4(aPos, 0.0, 1.0);

	o_Color = aColor;
}