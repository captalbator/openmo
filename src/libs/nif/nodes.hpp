#pragma once

#include "objects.hpp"

namespace nif {

enum SortingMode : uint32_t {
    SORTING_INHERIT,
    SORTING_OFF,
};

struct NiDynamicEffect;

struct NiNode : public NiAVObject {
    NiBlockList<NiAVObject> children;
    NiBlockList<NiDynamicEffect> effects;

    void read(NifStream* nif) override;
    void post(NifReader& nif) override;
};

struct NiBillboardNode : public NiNode {};

struct NiSortAdjustNode : public NiNode {
    SortingMode sortingMode;

    void read(NifStream* nif) override;
};

} // namespace nif

