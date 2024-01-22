#pragma once

namespace nif
{

struct Transform
{
    glm::mat3x3 rotation;
    glm::vec3 translation;
    float scale;
};

struct ByteColor3
{
    uint8_t r, g, b = 0;

    ByteColor3() = default;
    ByteColor3(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

struct ByteColor4
{
    uint8_t r, g, b, a = 0;

    ByteColor4() = default;
    ByteColor4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
};

struct NiBound
{
    glm::vec3 center;
    float radius;
};

} // namespace nif
