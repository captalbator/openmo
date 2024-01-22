#include "shaders.hpp"

namespace nif
{

void TexDesc::read(NifStream *nif)
{
    inUse = nif->getBoolean32();
    if (!inUse)
        return;

    textureSource.read(nif);
    clampMode = (TexClampMode)nif->getUint32();
    filterMode = (TexFilterMode)nif->getUint32();
    uvSet = nif->getUint32();
    ps2_l = nif->getInt16();
    ps2_k = nif->getInt16();
    nif->skip(2); // unknown short
}

void TexDesc::post(NifReader &nif)
{
    textureSource.post(nif);
}

void NiAlphaProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
    threshold = nif->getUint8();
}

void NiShaderProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
}

void NiSpecularProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
}

void NiZBufferProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
}

void NiDitherProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
}

void NiFogProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
    fogDepth = nif->getFloat();
    fogColor = nif->getVector3();
}

void NiWireframeProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();
}

void NiMaterialProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();

    ambientColor = nif->getVector3();
    diffuseColor = nif->getVector3();
    specularColor = nif->getVector3();
    emissiveColor = nif->getVector3();

    glossiness = nif->getFloat();
    alpha = nif->getFloat();
}

void NiVertexColorProperty::read(NifStream *nif)
{
    NiProperty::read(nif);

    flags = nif->getUint16();

    vertexMode = nif->getUint32();
    lightingMode = nif->getUint32();
}

void NiTexturingProperty::read(NifStream *nif)
{
    NiProperty::read(nif);
    flags = nif->getUint16();
    applyMode = nif->getUint32();
    textureCount = nif->getUint32();

    if (!textureCount)
        return;

    textures.resize(textureCount);
    for (uint32_t i = 0; i < textureCount; i++)
    {
        textures[i].read(nif);
        if (i == 5 && textures[i].inUse)
        {
            envMapLumaBias = nif->getVector2();
            bumpMapMatrix = nif->getVector4();
        }
    }
}

void NiTexturingProperty::post(NifReader &nif)
{
    NiProperty::post(nif);
    for (auto &texture : textures)
        texture.post(nif);
}

} // namespace nif
