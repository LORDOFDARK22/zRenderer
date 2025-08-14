#version 330 core

out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;

uniform sampler2D o_Texture;

void main()
{
	//FragColor = texture(o_Texture, o_UV) * o_Color;
	FragColor = o_Color;
}