#include "objects.hpp"
#include "shaders.hpp"

namespace nif {

void NiBoundingVolume::read(NifStream* nif)
{
    type = (Type)nif->getUint32();
    switch (type) {
        case BASE_BV:
            break;
        case SPHERE_BV:
            sphere.center = nif->getVector3();
            sphere.radius = nif->getFloat();
            break;
        case BOX_BV:
            box.center = nif->getVector3();
            box.axes = nif->getMatrix33();
            box.extents = nif->getVector3();
            break;
        case CAPSULE_BV:
            capsule.center = nif->getVector3();
            capsule.axis = nif->getVector3();
            capsule.extent = nif->getFloat();
            capsule.radius = nif->getFloat();
            break;
        case LOZENGE_BV:
            lozenge.radius = nif->getFloat();
            lozenge.center = nif->getVector3();
            lozenge.axis0 = nif->getVector3();
            lozenge.axis1 = nif->getVector3();
            break;
        case UNION_BV:
        {
            uint32_t n_children = nif->getUint32();
            if (n_children == 0)
                break;
            children.resize(n_children);
            for (NiBoundingVolume& child : children)
                child.read(nif);
            break;
        }
        case HALFSPACE_BV:
            halfSpace.plane = NiPlane(nif->getVector4());
            break;
        default:
            break;
    }
}

void NiObjectNET::read(NifStream* nif)
{
    name = nif->getString();
    extraData.read(nif);
    controller.read(nif);
}

void NiObjectNET::post(NifReader& nif)
{
    extraData.post(nif);
    controller.post(nif);
}

void NiAVObject::read(NifStream* nif)
{
    NiObjectNET::read(nif);

    flags = nif->getUint16();
    transform.translation = nif->getVector3();
    transform.rotation = nif->getMatrix33();
    transform.scale = nif->getFloat();
    velocity = nif->getVector3();
    readBlockList(nif, properties);
    hasBoundingVolume = nif->getBoolean32();
    if (hasBoundingVolume)
        boundingVolume.read(nif);
}

void NiAVObject::post(NifReader& nif)
{
    NiObjectNET::post(nif);
    postBlockList(nif, properties);
}

void NiPalette::read(NifStream* nif)
{
    uint32_t alphaMask = !nif->getChar() ? 0xFF000000 : 0;
    palette.resize(256);
    uint32_t nEntries = nif->getUint32();
    for (uint32_t i = 0; i < nEntries; i++)
        palette[i] = nif->getUint32() | alphaMask;
}

void NiPixelData::read(NifStream* nif)
{
    pixelFormat = (PixelFormat)nif->getUint32();
    redMask = nif->getUint32();
    greenMask = nif->getUint32();
    blueMask = nif->getUint32();
    alphaMask = nif->getUint32();
    bitsPerPixel = nif->getUint32();
    nif->skip(8); // old_fast_compare
    
    palette.read(nif);

    nMipmaps = nif->getUint32();

    nif->getUint32(); // bytes per pixel.

    for (uint32_t i = 0; i < nMipmaps; i++) {
        Mipmap m;
        m.width = nif->getUint32();
        m.height = nif->getUint32();
        m.dataOffset = nif->getUint32();
        mipmaps.push_back(m);
    }

    uint32_t nPixels = nif->getUint32();
    if (nPixels)
        nif->getUint8s(data, nPixels);
}

void NiPixelData::post(NifReader& nif)
{
    palette.post(nif);
}

void NiSourceTexture::read(NifStream* nif)
{
    NiTexture::read(nif);

    isExternal = nif->getChar() != 0;
    bool internal = false;
    if (isExternal)
        filename = nif->getString();
    else {
        internal = nif->getChar();
    }

    if (!isExternal && internal)
        textureData.read(nif);

    pixelLayout = (PixelLayout)nif->getUint32();
    mipmap = (MipmapFormat)nif->getUint32();
    alphaFormat = (AlphaFormat)nif->getUint32();

    isStatic = nif->getChar();
}

void NiSourceTexture::post(NifReader& nif)
{
    NiTexture::post(nif);

    textureData.post(nif);
}

} // namespace nif
