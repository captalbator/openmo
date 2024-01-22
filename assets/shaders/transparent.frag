#version 440 core

layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

in vec4 fragPos;
in vec3 fragNormal;
in vec2 texCoords;

uniform sampler2D uTexture;

void main()
{
    vec4 color = texture(uTexture, texCoords);

    float z = gl_FragCoord.z / gl_FragCoord.w;
    float weight =
        max(min(1.0, max(max(color.r, color.g), color.b) * color.a), color.a) *
        clamp(0.03 / (1e-5 + pow(z / 200, 4.0)), 1e-2, 3e3);

    accum = vec4(color.rgb * color.a, color.a) * weight;
    reveal = color.a;
}