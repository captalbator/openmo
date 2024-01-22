#include "particles.hpp"

namespace nif {

void NiParticlesData::read(NifStream* nif)
{
    NiGeometryData::read(nif);

    nParticles = nif->getUint16();
    particleRadius = nif->getFloat();
    nActive = nif->getUint16();
    hasSizes = nif->getBoolean32();
    if (hasSizes)
        nif->getFloats(sizes, nParticles);
}

} // namespace nif
