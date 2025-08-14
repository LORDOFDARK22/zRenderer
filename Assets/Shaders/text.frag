#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;
in float o_FontIndex;

uniform sampler2D u_FontsAtlas[32];

void main()
{
	int index = int(o_FontIndex);

	float alpha = texture(u_FontsAtlas[index], o_UV).r;
    FragColor = vec4(o_Color.rgb, o_Color.a * alpha);
}