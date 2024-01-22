#include "nodes.hpp"
#include "effects.hpp"

namespace nif {

void NiNode::read(NifStream* nif)
{
    NiAVObject::read(nif);
    readBlockList(nif, children);
    readBlockList(nif, effects);
}

void NiNode::post(NifReader& nif)
{
    NiAVObject::post(nif);
    postBlockList(nif, children);
    postBlockList(nif, effects);
}

void NiSortAdjustNode::read(NifStream* nif)
{
    NiNode::read(nif);
    sortingMode = (SortingMode)nif->getUint32();
}

} // namespace nif
