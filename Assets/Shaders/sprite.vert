#version 460

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;

out vec2 uv;
out vec4 OutColor;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	gl_Position = projection * model * vec4(aPosition, 0.0, 1.0);
	uv = aUV;
	OutColor = aColor;
}