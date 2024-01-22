#version 400 core
out vec4 fragColor;

in vec4 fragPos;
in vec3 fragNormal;
in vec2 texCoords;

uniform vec3 uViewPos;

uniform sampler2D uTexture;

void main() {
    vec4 texColor = texture(uTexture, texCoords);
    
    fragColor = texColor;
}