#include "controller.hpp"

#include "nifstream.hpp"

namespace nif
{

void NiTimeController::read(NifStream *nif)
{
    nextController.read(nif);

    flags = nif->getUint16();
    frequency = nif->getFloat();
    phase = nif->getFloat();
    startTime = nif->getFloat();
    stopTime = nif->getFloat();

    target.read(nif);
}

void NiTimeController::post(NifReader &nif)
{
    NiObject::post(nif);
    nextController.post(nif);
    target.post(nif);
}

void NiEmitterModifier::read(NifStream *nif)
{
    nextModifier.read(nif);
    controller.read(nif);
}

void NiEmitterModifier::post(NifReader &nif)
{
    nextModifier.post(nif);
    controller.post(nif);
}

void NiParticleModifier::read(NifStream *nif)
{
    nextModifier.read(nif);
    controller.read(nif);
}

void NiParticleModifier::post(NifReader &nif)
{
    nextModifier.post(nif);
    controller.post(nif);
}

void NiParticleGrowFade::read(NifStream *nif)
{
    NiParticleModifier::read(nif);
    grow = nif->getFloat();
    fade = nif->getFloat();
}

void NiParticleCollider::read(NifStream *nif)
{
    NiParticleModifier::read(nif);

    bounce = nif->getFloat();
}

void NiParticleSystemController::read(NifStream *nif)
{
    NiTimeController::read(nif);

    speed = nif->getFloat();
    speedVariation = nif->getFloat();
    declination = nif->getFloat();
    declinationVariation = nif->getFloat();
    planarAngle = nif->getFloat();
    planarAngleVariation = nif->getFloat();
    initialNormal = nif->getVector3();
    initialColor = nif->getVector4();
    initialSize = nif->getFloat();
    emitStartTime = nif->getFloat();
    emitStopTime = nif->getFloat();
    resetParticleSystem = nif->getUint8();
    birthRate = nif->getFloat();
    lifetime = nif->getFloat();
    lifetimeVariation = nif->getFloat();
    if (nif->getUint8() > 0)
        useBirthRate = true;
    if (nif->getUint8() > 0)
        spawnOnDeath = true;

    emitterDirections = nif->getVector3();

    emitter.read(nif);

    nSpawnGens = nif->getUint16();
    percentageSpawned = nif->getFloat();
    spawnMult = nif->getUint16();
    spawnSpeedChaos = nif->getFloat();
    spawnDirChaos = nif->getFloat();

    nParticles = nif->getUint16();
    nValid = nif->getUint16();

    particles.resize(nParticles);
    for (size_t i = 0; i < nParticles; i++)
    {
        particles[i].velocity = nif->getVector3();
        particles[i].rotationAxis = nif->getVector3();
        particles[i].age = nif->getFloat();
        particles[i].lifespan = nif->getFloat();
        particles[i].lastUpdate = nif->getFloat();
        particles[i].spawnGen = nif->getUint16();
        particles[i].code = nif->getUint16();
    }

    emitterModifier.read(nif);
    particleModifier.read(nif);
    particleCollider.read(nif);
}

void NiParticleSystemController::post(NifReader &nif)
{
    NiTimeController::post(nif);
    emitter.post(nif);
    emitterModifier.post(nif);
    particleModifier.post(nif);
    particleCollider.post(nif);
}

void NiUVData::read(NifStream *nif)
{
    uTranslation = std::make_shared<FloatKeyMap>();
    vTranslation = std::make_shared<FloatKeyMap>();
    uScale = std::make_shared<FloatKeyMap>();
    vScale = std::make_shared<FloatKeyMap>();

    uTranslation->read(nif);
    vTranslation->read(nif);
    uScale->read(nif);
    vScale->read(nif);
}

void NiUVController::read(NifStream *nif)
{
    NiTimeController::read(nif);

    textureSet = nif->getUint16();
    data.read(nif);
}

void NiUVController::post(NifReader &nif)
{
    NiTimeController::post(nif);
    data.post(nif);
}

void NiMorphData::read(NifStream *nif)
{
    nMorphs = nif->getUint32();
    nVertices = nif->getUint32();
    relativeTargets = nif->getUint8();

    morphs.resize(nMorphs);
    for (size_t i = 0; i < nMorphs; i++)
    {
        morphs[i].keyFrames = std::make_unique<FloatKeyMap>();
        morphs[i].keyFrames->read(nif);
        nif->getVector3s(morphs[i].vertices, nVertices);
    }
}

void NiGeomMorpherController::read(NifStream *nif)
{
    NiInterpController::read(nif);
    data.read(nif);
}

void NiGeomMorpherController::post(NifReader &nif)
{
    NiInterpController::post(nif);
    data.post(nif);
}

void NiFlipController::read(NifStream *nif)
{
    NiFloatInterpController::read(nif);
    textureSlot = (TexType)nif->getUint32();
    accumTime = nif->getFloat();
    delta = nif->getFloat();

    readBlockList(nif, sources);
}

void NiFlipController::post(NifReader &nif)
{
    NiFloatInterpController::post(nif);
    postBlockList(nif, sources);
}

} // namespace nif
