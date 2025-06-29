#version 460

out vec4 FragColor;

in vec4 OutColor;

void main()
{
	FragColor = OutColor;
}