#version 330 core

in vec2 vUV;
in vec4 vColor;
in float vTexID;

uniform sampler2D u_Textures[32];

out vec4 FragColor;

void main()
{
    int id = int(vTexID);
    FragColor = texture(u_Textures[id], vUV) * vColor;
}