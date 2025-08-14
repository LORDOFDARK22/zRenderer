#version 330 core

out vec4 FragColor;

in vec2 UV;
in vec4 Color;
in float indexFont;

uniform sampler2D u_FontsAtlas[32];

void main() 
{
    int index = int(indexFont);

    float alpha = texture(u_FontsAtlas[index], UV).r;
    FragColor = vec4(Color.rgb, Color.a * alpha);
}