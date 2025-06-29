#version 460

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;

uniform mat4 projection;

//Output
out vec4 OutColor;

void main()
{
	gl_Position = projection * vec4(aPosition, 0.0, 1.0);
	OutColor = aColor;
}