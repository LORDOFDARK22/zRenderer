#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in float a_FontIndex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 o_Color;
out vec2 o_UV;
out float o_FontIndex;

void main()
{
	gl_Position = projection * view * model * vec4(a_Pos, 1.0);

	o_Color = a_Color;
	o_UV = a_UV;
	o_FontIndex = a_FontIndex;
}