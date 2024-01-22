#pragma once

#include "types.hpp"
#include "nodes.hpp"
#include "blockptr.hpp"

namespace nif {

struct BoneVertData {
    uint16_t index;
    float weight;
};

struct NiSkinData : public NiObject {
    struct BoneData {
        Transform skinTransform;
        NiBound boundingSphere;
        std::vector<BoneVertData> vertexWeights;
    };

    Transform skinTransform;
    uint32_t nBones;
    std::vector<BoneData> bones;
    
    void read(NifStream* nif) override;
};

struct NiSkinInstance : public NiObject {
    NiBlockPtr<NiSkinData> data;
    NiBlockPtr<NiNode> skeletonRoot;
    NiBlockList<NiNode> boneList;

    void read(NifStream* nif) override;
    void post(NifReader& reader) override;
};

} // namespace nif
