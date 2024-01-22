#version 400 core

in vec2 texCoords;

layout (location = 0) out vec4 frag;

uniform sampler2D screen;

void main()
{
    frag = vec4(texture(screen, texCoords).rgb, 1.0);
}