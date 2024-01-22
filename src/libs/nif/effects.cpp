#include "effects.hpp"

namespace nif
{

void NiDynamicEffect::read(NifStream *nif)
{
    NiAVObject::read(nif);

    readBlockList(nif, affectedNodes);
#if 0
    uint32_t nAffected = nif->getUint32();
    for (uint32_t i = 0; i < nAffected; i++)
        nif->getUint32();
#endif
}

void NiDynamicEffect::post(NifReader &nif)
{
    NiAVObject::post(nif);

    postBlockList(nif, affectedNodes);
}

void NiTextureEffect::read(NifStream *nif)
{
    NiDynamicEffect::read(nif);

    modelProjectionMatrix = nif->getMatrix33();
    modelProjectionTranslation = nif->getVector3();

    textureFiltering = (TexFilterMode)nif->getUint32();
    textureClamping = (TexClampMode)nif->getUint32();
    textureType = (EffectType)nif->getUint32();
    coordGenType = (CoordGenType)nif->getUint32();

    sourceTexture.read(nif);

    enablePlane = nif->getBoolean8();
    plane.normal = nif->getVector3();
    plane.constant = nif->getFloat();
    ps2_l = nif->getUint16();
    ps2_k = nif->getUint16();
    nif->skip(2); // unknown short;
}

void NiTextureEffect::post(NifReader &nif)
{
    NiDynamicEffect::post(nif);

    sourceTexture.post(nif);
}

void NiLight::read(NifStream *nif)
{
    NiDynamicEffect::read(nif);

    dimmer = nif->getFloat();
    ambient = nif->getVector3();
    diffuse = nif->getVector3();
    specular = nif->getVector3();
}

void NiPointLight::read(NifStream *nif)
{
    NiLight::read(nif);

    constantAttenuation = nif->getFloat();
    linearAttenuation = nif->getFloat();
    quadraticAttenuation = nif->getFloat();
}

void NiSpotLight::read(NifStream *nif)
{
    NiPointLight::read(nif);

    cutoff = nif->getFloat();
    exponent = nif->getFloat();
}

} // namespace nif
