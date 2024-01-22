#pragma once

#include "controller.hpp"
#include "objects.hpp"


namespace nif
{

// FIXME: Implement these
enum VertexMode : uint32_t
{
};
enum LightingMode : uint32_t
{
};
enum ApplyMode : uint32_t
{
};

struct TexDesc
{
    bool inUse;
    NiBlockPtr<NiSourceTexture> textureSource;
    TexType texType;
    TexClampMode clampMode = CLAMP_S_CLAMP_T;
    TexFilterMode filterMode = FILTER_TRILERP;
    uint32_t uvSet = 0;
    int16_t ps2_l = 0;
    int16_t ps2_k = -75;
    uint16_t unknown = 0;

    void read(NifStream *nif);
    void post(NifReader &nif);
};

struct NiProperty : public NiObjectNET
{
};

struct NiAlphaProperty : public NiProperty
{
    uint16_t flags = 0;
    uint8_t threshold = 128;

    void read(NifStream *nif) override;
};

struct NiShaderProperty : public NiProperty
{
    uint16_t flags = 0;

    void read(NifStream *nif) override;
};

struct NiSpecularProperty : public NiProperty
{
    uint16_t flags = 0;

    void read(NifStream *nif) override;
};

struct NiZBufferProperty : public NiProperty
{
    uint16_t flags = 3;

    void read(NifStream *nif) override;
};

struct NiDitherProperty : public NiProperty
{
    uint16_t flags = 0;

    void read(NifStream *nif) override;
};

struct NiFogProperty : public NiProperty
{
    uint16_t flags = 0;
    float fogDepth = 1.0f;
    glm::vec3 fogColor;

    void read(NifStream *nif) override;
};

struct NiWireframeProperty : public NiProperty
{
    uint16_t flags = 0;

    void read(NifStream *nif) override;
};

struct NiMaterialProperty : public NiProperty
{
    uint16_t flags = 0;
    glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 emissiveColor;
    float glossiness = 10.0f;
    float alpha = 1.0f;

    void read(NifStream *nif) override;
};

struct NiVertexColorProperty : public NiProperty
{
    uint16_t flags;
    uint32_t vertexMode = 0;
    uint32_t lightingMode = 0;

    void read(NifStream *nif) override;
};

struct NiTexturingProperty : public NiProperty
{
    uint16_t flags;
    uint32_t applyMode = 2;
    uint32_t textureCount = 7;
    std::vector<TexDesc> textures;

    glm::vec2 envMapLumaBias;
    glm::vec4 bumpMapMatrix;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

}; // namespace nif
