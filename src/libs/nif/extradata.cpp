#include "extradata.hpp"

namespace nif {

void Extra::read(NifStream* nif)
{
    next.read(nif);
    size = nif->getUint32();
}

void Extra::post(NifReader& nif)
{
    next.post(nif);
}

void NiExtraData::read(NifStream* nif)
{
    Extra::read(nif);
    if (size > 0) {
        bytes.resize(size);
        nif->getUint8s(bytes, size);
    }
}

void NiStringExtraData::read(NifStream* nif)
{
    Extra::read(nif);
    stringData = nif->getString();
}

} // namespace nif
