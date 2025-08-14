#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aCenter;
layout (location = 2) in float aRadius;
layout (location = 3) in vec4 aColor;

uniform mat4 projection;
uniform mat4 view;

out vec4 o_Color;

void main()
{
	vec2 pos = aPos * aRadius + aCenter; // تكبير وتحريك
    gl_Position = projection * view * vec4(pos, 0.0, 1.0);

	o_Color = aColor;
}