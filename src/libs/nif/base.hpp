#pragma once

#include <string>

namespace nif {

class NifStream;
class NifReader;

enum class NiType {
    Unknown = 0,
    NiObject,
    NiObjectNET,
    NiAVObject,
    NiNode,
    NiBillboardNode,
    NiSortAdjustNode,
    NiPalette,
    NiPixelData,
    NiTexture,
    NiSourceTexture,
    NiDynamicEffect,
    NiTextureEffect,
    NiLight,
    NiAmbientLight,
    NiDirectionalLight,
    NiPointLight,
    NiSpotLight,
    NiTimeController,
    NiEmitterModifier,
    NiParticleModifier,
    NiParticleGrowFade,
    NiParticleCollider,
    NiParticleSystemController,
    NiUVData,
    NiUVController,
    NiMorphData,
    NiInterpController,
    NiGeomMorpherController,
    // FIXME: Unused?
    NiSingleInterpController,
    NiFloatInterpController,

    NiFlipController,
    NiExtraData,
    NiStringExtraData,
    NiGeometry,
    NiGeometryData,
    NiTriBasedGeom,
    NiTriBasedGeomData,
    NiTriShapeData,
    NiTriShape,
    NiParticles,
    NiParticlesData,
    NiAutoNormalParticles,
    NiAutoNormalParticlesData,
    NiProperty,
    NiAlphaProperty,
    NiShaderProperty,
    NiSpecularProperty,
    NiZBufferProperty,
    NiDitherProperty,
    NiFogProperty,
    NiWireframeProperty,
    NiMaterialProperty,
    NiVertexColorProperty,
    NiTexturingProperty,
    NiSkinData,
    NiSkinInstance,
};

struct NiObject {
    NiType type{NiType::Unknown};
    std::string typeName;
    uint32_t index{~0u};

    NiObject() = default;

    // Parse from file
    virtual void read(NifStream *nif) = 0;
    // Post processing
    virtual void post(NifReader &nif) {}

    virtual ~NiObject() {}
};

} // namespace nif
