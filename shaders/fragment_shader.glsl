#version 330 core

out vec4 FragColor;

in vec4 ourColor;
in vec2 UV;
in vec4 ourPos;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, UV) * ourColor;
}