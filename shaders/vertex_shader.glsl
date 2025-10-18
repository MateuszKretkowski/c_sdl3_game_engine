#version 330 core
layout (location = 0) in mat4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

out vec4 ourColor;
out vec2 UV;
out mat4 ourPos;

void main()
{
    ourPos = aPos;
    ourColor = aColor;
    UV = aUV;
}