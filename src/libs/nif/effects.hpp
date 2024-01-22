#pragma once

#include "nodes.hpp"

namespace nif
{

struct NiDynamicEffect : public NiAVObject
{
    NiBlockList<NiNode> affectedNodes;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

// FIXME: Clean this up a bit
struct NiTextureEffect : public NiDynamicEffect
{
    glm::mat3x3 modelProjectionMatrix;
    glm::vec3 modelProjectionTranslation;
    TexFilterMode textureFiltering = FILTER_TRILERP;
    TexClampMode textureClamping = WRAP_S_WRAP_T;
    EffectType textureType;
    CoordGenType coordGenType = CG_SPHERE_MAP;
    NiBlockPtr<NiSourceTexture> sourceTexture;
    bool enablePlane = false;
    NiPlane plane;
    int16_t ps2_l = 0;
    int16_t ps2_k = -75;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct NiLight : public NiDynamicEffect
{
    float dimmer = 1.0f;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    void read(NifStream *nif) override;
};

struct NiAmbientLight : public NiLight
{
};

struct NiDirectionalLight : public NiLight
{
};

struct NiPointLight : public NiLight
{
    float constantAttenuation = 0.0f;
    float linearAttenuation = 0.0f;
    float quadraticAttenuation = 0.0f;

    void read(NifStream *nif) override;
};

struct NiSpotLight : public NiPointLight
{
    float cutoff = 0.0f;
    float exponent = 1.0f;

    void read(NifStream *nif) override;
};

} // namespace nif
