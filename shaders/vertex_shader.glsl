#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

uniform float horizontal_offset;

out vec4 ourColor;
out vec2 UV;
out vec4 ourPos;

void main()
{
    gl_Position = vec4(aPos.x + horizontal_offset, -aPos.y, aPos.z, 1.0);
    ourPos = gl_Position;
    ourColor = aColor;
    UV = aUV;
}