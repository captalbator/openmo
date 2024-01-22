#version 400 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;

out vec2 texCoords;

void main()
{
    texCoords = aUV;
    gl_Position = vec4(aPosition, 1.0);
}
