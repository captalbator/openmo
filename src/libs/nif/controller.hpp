#pragma once

#include "base.hpp"
#include "blockptr.hpp"
#include "keys.hpp"
#include "objects.hpp"


namespace nif
{

enum TexType : uint32_t
{
    BASE_MAP,
    DARK_MAP,
    DETAIL_MAP,
    GLOSS_MAP,
    GLOW_MAP,
    BUMP_MAP,
    DECAL_0_MAP,
    DECAL_1_MAP,
    DECAL_2_MAP
};

struct NiParticleSystemController;

struct NiTimeController : public NiObject
{
    NiBlockPtr<NiTimeController> nextController;
    uint16_t flags;
    float frequency = 1.0f;
    float phase = 0.0f;
    float startTime = 0.0f;
    float stopTime = 0.0f;
    NiBlockPtr<NiObjectNET> target;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct NiParticleInfo
{
    glm::vec3 velocity;
    glm::vec3 rotationAxis;
    float age;
    float lifespan;
    float lastUpdate;
    uint16_t spawnGen = 0;
    uint16_t code;
};

struct NiEmitterModifier : public NiObject
{
    NiBlockPtr<NiEmitterModifier> nextModifier;
    NiBlockPtr<NiParticleSystemController> controller;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct NiParticleModifier : public NiObject
{
    NiBlockPtr<NiParticleModifier> nextModifier;
    NiBlockPtr<NiParticleSystemController> controller;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct NiParticleGrowFade : public NiParticleModifier
{
    float grow;
    float fade;

    void read(NifStream *nif) override;
};

struct NiParticleCollider : public NiParticleModifier
{
    float bounce;

    void read(NifStream *nif) override;
};

struct NiParticleSystemController : public NiTimeController
{
    float speed;
    float speedVariation;
    float declination; // verticle emit direction in radians (0.0 up; 1.6 horizontal; 3.1416 : down)
    float declinationVariation;
    float planarAngle;
    float planarAngleVariation;
    glm::vec3 initialNormal;
    glm::vec4 initialColor;
    float initialSize = 1.0f;
    float emitStartTime;
    float emitStopTime;
    uint8_t resetParticleSystem;
    float birthRate;
    float lifetime;
    float lifetimeVariation;
    bool useBirthRate;
    bool spawnOnDeath;
    glm::vec3 emitterDirections;
    NiBlockPtr<NiAVObject> emitter;
    uint16_t nSpawnGens = 1;
    float percentageSpawned = 1.0f;
    uint16_t spawnMult;
    float spawnSpeedChaos;
    float spawnDirChaos;
    uint16_t nParticles;
    uint16_t nValid;
    std::vector<NiParticleInfo> particles;
    NiBlockPtr<NiEmitterModifier> emitterModifier;
    NiBlockPtr<NiParticleModifier> particleModifier;
    NiBlockPtr<NiParticleCollider> particleCollider;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct NiUVData : public NiObject
{
    FloatKeyMapPtr uTranslation;
    FloatKeyMapPtr vTranslation;
    FloatKeyMapPtr uScale;
    FloatKeyMapPtr vScale;

    void read(NifStream *nif) override;
};

struct NiUVController : public NiTimeController
{
    uint16_t textureSet = 0;
    NiBlockPtr<NiUVData> data;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct Morph
{
    FloatKeyMapPtr keyFrames;
    std::vector<glm::vec3> vertices;
};

struct NiMorphData : public NiObject
{
    uint32_t nMorphs = 0;
    uint32_t nVertices = 0;
    // FIXME: make sure this is always 1 so we can safely get rid of it
    uint8_t relativeTargets = 1;
    std::vector<Morph> morphs;

    void read(NifStream *nif) override;
};

struct NiInterpController : public NiTimeController
{
};

struct NiGeomMorpherController : public NiInterpController
{
    NiBlockPtr<NiMorphData> data;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

struct NiSingleInterpController : public NiInterpController
{
};
struct NiFloatInterpController : public NiSingleInterpController
{
};

struct NiFlipController : public NiFloatInterpController
{
    TexType textureSlot = BASE_MAP;
    float accumTime;
    float delta;
    NiBlockList<NiSourceTexture> sources;

    void read(NifStream *nif) override;
    void post(NifReader &nif) override;
};

} // namespace nif
