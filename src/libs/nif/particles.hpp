#pragma once

#include "geom.hpp"

namespace nif {

struct NiParticlesData : public NiGeometryData {
    uint16_t nParticles;
    float particleRadius;
    uint16_t nActive;
    bool hasSizes;
    std::vector<float> sizes;

    void read(NifStream* nif) override;
};

struct NiParticles : public NiGeometry {};

struct NiAutoNormalParticles : public NiParticles {};

// FIXME: Possibly unused?
struct NiAutoNormalParticlesData : public NiParticlesData {};


} // namespace nif
