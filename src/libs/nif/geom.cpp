#include "geom.hpp"

namespace nif
{

void NiGeometryData::read(NifStream *nif)
{
    uint16_t nVertices = nif->getUint16();
    hasVertices = nif->getBoolean32();
    if (hasVertices)
    {
        nif->getVector3s(vertices, nVertices);
    }

    hasNormals = nif->getBoolean32();
    if (hasNormals)
    {
        nif->getVector3s(normals, nVertices);
    }

    boundingSphere.center = nif->getVector3();
    boundingSphere.radius = nif->getFloat();

    hasVertexColors = nif->getBoolean32();
    if (hasVertexColors)
    {
        nif->getVector4s(vertexColors, nVertices);
    }

    nUVSets = nif->getUint16();

    hasUVs = nif->getBoolean32();
    if (hasUVs)
    {
        uvSets.resize(nUVSets);
        for (uint32_t i = 0; i < nUVSets; i++)
        {
            nif->getVector2s(uvSets[i], nVertices);
#if 0
            // Flip coordinates to OpenGL space
            for (uint32_t uv = 0; uv < uvSets[i].size(); ++uv) {
                uvSets[i][uv] = glm::vec2(uvSets[i][uv].x, 1.f - uvSets[i][uv].y);
            }
#endif
        }
    }
}

void NiTriBasedGeomData::read(NifStream *nif)
{
    NiGeometryData::read(nif);
    nTriangles = nif->getUint16();
};

void NiTriShapeData::read(NifStream *nif)
{
    NiTriBasedGeomData::read(nif);
    nTrianglePoints = nif->getUint32();
    nif->getUint16s(triangles, nTrianglePoints);

    nMatchGroups = nif->getUint16();
    if (nMatchGroups > 0)
    {
        matchGroups.resize(nMatchGroups);
        for (uint16_t i = 0; i < nMatchGroups; i++)
        {
            matchGroups[i].count = nif->getUint16();
            nif->getUint16s(matchGroups[i].matches, matchGroups[i].count);
        }
    }
}

void NiGeometry::read(NifStream *nif)
{
    NiAVObject::read(nif);
    data.read(nif);
    skinInstance.read(nif);
}

void NiGeometry::post(NifReader &nif)
{
    NiAVObject::post(nif);
    data.post(nif);
    skinInstance.post(nif);
}

}; // namespace nif
