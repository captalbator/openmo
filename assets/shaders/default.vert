#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec4 fragPos;
out vec4 vertexColor;

void main() {
    fragPos = vec4(aPosition, 1.0);
    vertexColor = vec4(aColor, 1.0);
    gl_Position = fragPos;
}
