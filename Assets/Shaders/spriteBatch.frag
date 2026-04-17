#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;
in float o_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(o_TexIndex);

	FragColor = texture(u_Textures[index], o_UV) * o_Color;
}