#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in float a_TexIndex;

out vec4 o_Color;
out vec2 o_UV;
out float o_TexIndex;
out vec2 o_FragPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	o_Color = a_Color;
	o_UV = a_UV;
	o_TexIndex = a_TexIndex;
	o_FragPos = a_Pos;

	gl_Position = projection * view * vec4(a_Pos, 0.0, 1.0);
}