#pragma once

#include "base.hpp"
#include "blockptr.hpp"
#include "extradata.hpp"
#include "types.hpp"

namespace nif {

enum PixelFormat : uint32_t {
    FMT_RGB,
    FMT_RGBA,
    FMT_PAL,
    FMT_PALA,
    FMT_DXT1,
    FMT_DXT3,
    FMT_DXT5,
    FMT_RBG24NONINT,
    FMT_BUMP,
    FMT_BUMPLUMA,
};

enum AlphaFormat : uint32_t {
    ALPHA_NONE,
    ALPHA_BINARY,
    ALPHA_SMOOTH,
    ALPHA_DEFAULT,
};

enum MipmapFormat : uint32_t { MIP_FMT_NO, MIP_FMT_YES, MIP_FMT_DEFAULT };

enum PixelLayout : uint32_t {
    PX_LAY_PALETTIZED_8,
    PX_LAY_HIGH_COLOR_16,
    PX_LAY_TRUE_COLOR_32,
    PX_LAY_COMPRESSED,
    PX_LAY_BUMPMAP,
    PX_LAY_PALETTIZED_4,
    PX_LAY_DEFAULT,
    PX_LAY_SINGLE_COLOR_8,
    PX_LAY_SINGLE_COLOR_16,
    PX_LAY_SINGLE_COLOR_32,
    PX_LAY_DOUBLE_COLOR_32,
    PX_LAY_DOUBLE_COLOR_64,
    PX_LAY_FLOAT_COLOR_32,
    PX_LAY_FLOAT_COLOR_64,
    PX_LAY_FLOAT_COLOR_128,
    PX_LAY_SINGLE_COLOR_4,
    PX_LAY_DEPTH_24_X8,
};

enum TexFilterMode : uint32_t {
    FILTER_NEAREST,
    FILTER_BILERP,
    FILTER_TRILERP,
    FILTER_NEAREST_MIPNEAREST,
    FILTER_NEAREST_MIPLERP,
    FILTER_BILERP_MIPNEAREST
};

enum TexClampMode : uint32_t { CLAMP_S_CLAMP_T, CLAMP_S_WRAP_T, WRAP_S_CLAMP_T, WRAP_S_WRAP_T };

enum EffectType : uint32_t {
    EFFECT_PROJECTED_LIGHT,
    EFFECT_PROJECTED_SHADOW,
    EFFECT_ENVIRONMENT_MAP,
    EFFECT_FOG_MAP
};

enum CoordGenType : uint32_t {
    CG_WORLD_PARALLEL,
    CG_WORLD_PERSPECTIVE,
    CG_SPHERE_MAP,
    CG_SPECULAR_CUBE_MAP,
    CG_DIFFUSE_CUBE_MAP
};

struct MipMapInfo {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t offset = 0;
};

struct NiPlane {
    glm::vec3 normal;
    float constant = 0.0f;

    NiPlane() = default;
    NiPlane(glm::vec4 vec)
    {
        normal = glm::vec3(vec.x, vec.y, vec.z);
        constant = vec.w;
    }
};

struct NiBoundingVolume {
    enum Type : uint32_t {
        BASE_BV = 0xFFFFFFFF,
        SPHERE_BV = 0,
        BOX_BV = 1,
        CAPSULE_BV = 2,
        LOZENGE_BV = 3,
        UNION_BV = 4,
        HALFSPACE_BV = 5
    };

    struct NiSphereBV {
        glm::vec3 center;
        float radius;
    };

    struct NiBoxBV {
        glm::vec3 center;
        glm::mat3x3 axes;
        glm::vec3 extents;
    };

    struct NiCapsuleBV {
        glm::vec3 center, axis;
        float extent{0.0f}, radius{0.0f};
    };

    struct NiLozengeBV {
        float radius{0.0f};
        glm::vec3 center, axis0, axis1;
    };

    struct NiHalfSpaceBV {
        NiPlane plane;
    };

    Type type;
    NiSphereBV sphere;
    NiBoxBV box;
    NiCapsuleBV capsule;
    NiLozengeBV lozenge;
    std::vector<NiBoundingVolume> children;
    NiHalfSpaceBV halfSpace;

    void read(NifStream* nif);
};

struct NiProperty;
struct NiTimeController;

struct NiObjectNET : public NiObject {
    std::string name;
    NiBlockPtr<NiExtraData> extraData;
    NiBlockPtr<NiTimeController> controller;

    void read(NifStream* nif) override;
    void post(NifReader& nif) override;
};

struct NiAVObject : public NiObjectNET {
    uint16_t flags;
    Transform transform;
    glm::vec3 velocity;
    NiBlockList<NiProperty> properties;
    uint32_t hasBoundingVolume;
    NiBoundingVolume boundingVolume;

    void read(NifStream* nif) override;
    void post(NifReader& nif) override;
};

// TODO: implement camera node
struct NiCamera : public NiAVObject {};

struct NiSequenceStreamHelper : public NiObjectNET {};

struct NiPalette : public NiObject {
    std::vector<uint32_t> palette;

    void read(NifStream* nif) override;
};

struct NiPixelData : public NiObject {
    PixelFormat pixelFormat;
    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t alphaMask;
    uint32_t bitsPerPixel;

    NiBlockPtr<NiPalette> palette;
    uint32_t nMipmaps{0};

    struct Mipmap {
        uint32_t width, height;
        uint32_t dataOffset;
    };

    std::vector<Mipmap> mipmaps;

    std::vector<uint8_t> data;

    void read(NifStream* nif) override;
    void post(NifReader& nif) override;
};

struct NiTexture : NiObjectNET {};

struct NiSourceTexture : public NiTexture {
    bool isExternal;

    std::string filename;
    NiBlockPtr<NiPixelData> textureData;

    PixelLayout pixelLayout;
    MipmapFormat mipmap;
    AlphaFormat alphaFormat;

    bool isStatic = true;

    void read(NifStream* nif) override;
    void post(NifReader& nif) override;
};

} // namespace nif
