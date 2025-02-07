#version 400 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec4 fragPos;
flat out vec3 fragNormal;
out vec2 texCoords;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    fragPos = vec4(aPosition, 1.0);

    fragNormal = aNormal;

    texCoords = aTexCoords;

    gl_Position = uProjection * uView * fragPos;
}
