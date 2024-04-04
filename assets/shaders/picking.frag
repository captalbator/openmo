#version 400 core

uniform uint uObjectIndex;
// uniform uint uDrawIndex;

out uvec3 FragColor;

void main()
{
    // TODO: uDrawIndex is omitted because we don't use it.
    FragColor = uvec3(uObjectIndex, 0, gl_PrimitiveID);
}