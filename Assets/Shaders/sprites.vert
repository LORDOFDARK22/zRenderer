#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTexID;

out vec2 vUV;
out vec4 vColor;
out float vTexID;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vUV = aUV;
    vColor = aColor;
    vTexID = aTexID;
    gl_Position = projection * view * vec4(aPos, 0.0, 1.0);
}