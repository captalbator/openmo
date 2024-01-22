#version 440 core

layout (location = 0) out vec4 frag;

layout (binding = 0) uniform sampler2D accum;
layout (binding = 1) uniform sampler2D reveal;

const float EPSILON = 0.00001;

bool isApproxEqual(float a, float b)
{
    return abs(a-b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

float max3(vec3 v)
{
    return max(max(v.x, v.y), v.z);
}

void main()
{
    ivec2 coords = ivec2(gl_FragCoord.xy);

    float revealage = texelFetch(reveal, coords, 0).r;

    if (isApproxEqual(revealage, 1.0))
        discard;

    vec4 accumulation = texelFetch(accum, coords, 0);

    // Suppress overflow
    if (isinf(max3(abs(accumulation.rgb))))
        accumulation.rgb = vec3(accumulation.a);

    vec3 averageColor = accumulation.rgb / clamp(accumulation.a, 1e-3, 5e4);

    frag = vec4(averageColor, revealage);
}