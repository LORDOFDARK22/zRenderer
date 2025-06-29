#version 460

out vec4 FragColor;

in vec2 uv;
in vec4 OutColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, uv);// * OutColor;
}