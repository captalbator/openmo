#pragma once

#include "blockptr.hpp"
#include "skin.hpp"

namespace nif
{

struct NiGeometryData : public NiObject
{
    bool hasVertices = true;
    bool hasNormals = true;
    bool hasVertexColors = false;
    bool hasUVs = false;
    NiBound boundingSphere;
    uint16_t nUVSets;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> vertexColors;
    std::vector<std::vector<glm::vec2>> uvSets;

    void read(NifStream *nif) override;
};

struct NiTriBasedGeomData : public NiGeometryData
{
    uint16_t nTriangles = 0;

    void read(NifStream *nif) override;
};

struct MatchGroup
{
    uint16_t count = 0;
    std::vector<uint16_t> matches;
};

struct NiTriShapeData : public NiTriBasedGeomData
{
    uint32_t nTrianglePoints;
    bool hasTriangles = true;
    // TODO: Change this to triangle instead of uint16_t
    std::vector<uint16_t> triangles;

    uint16_t nMatchGroups;
    std::vector<MatchGroup> matchGroups;

    void read(NifStream *nif) override;
};

struct NiGeometry : public NiAVObject
{
    NiBlockPtr<NiGeometryData> data;
    NiBlockPtr<NiSkinInstance> skinInstance;

    void read(NifStream *nif) override;
    void post(NifReader &reader) override;
};

struct NiTriBasedGeom : public NiGeometry
{
};
struct NiTriShape : public NiTriBasedGeom
{
};

} // namespace nif
