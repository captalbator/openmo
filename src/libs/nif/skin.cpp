#include "skin.hpp"

namespace nif {

void NiSkinData::read(NifStream* nif)
{
    skinTransform.rotation = nif->getMatrix33();
    skinTransform.translation = nif->getVector3();
    skinTransform.scale = nif->getFloat();

    nBones = nif->getUint32();

    bones.resize(nBones);
    for (BoneData& data : bones) {
        data.skinTransform.rotation = nif->getMatrix33();
        data.skinTransform.translation = nif->getVector3();
        data.skinTransform.scale = nif->getFloat();
        data.boundingSphere.center = nif->getVector3();
        data.boundingSphere.radius = nif->getFloat();

        data.vertexWeights.resize(nif->getUint16());
        for (size_t j = 0; j < data.vertexWeights.size(); j++) {
            data.vertexWeights[j].index = nif->getUint16();
            data.vertexWeights[j].weight = nif->getFloat();
        }
    }
}

void NiSkinInstance::read(NifStream* nif)
{
    data.read(nif);
    skeletonRoot.read(nif);
    readBlockList(nif, boneList);
}

void NiSkinInstance::post(NifReader& nif)
{
    data.post(nif);
    skeletonRoot.post(nif);
    postBlockList(nif, boneList);

    for (auto& bone : boneList) {
        if (bone.empty())
            throw std::runtime_error("missing bone");

        // TODO: Implement setBone();
        //bone->setBone();
    }
}

};
