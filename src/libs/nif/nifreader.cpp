#include "nifreader.hpp"
#include "nifstream.hpp"

#include "libs/common/log.hpp"

#include "controller.hpp"
#include "effects.hpp"
#include "extradata.hpp"
#include "geom.hpp"
#include "nodes.hpp"
#include "particles.hpp"
#include "shaders.hpp"

namespace nif
{

template <typename T, NiType BlockType> static std::unique_ptr<NiObject> construct()
{
    auto obj = std::make_unique<T>();
    obj->type = BlockType;
    return obj;
}

using CreateBlock = std::unique_ptr<NiObject> (*)();

static std::map<std::string, CreateBlock> makeFactory()
{
    return {
        {"NiNode", &construct<NiNode, NiType::NiNode>},
        {"NiBillboardNode", &construct<NiBillboardNode, NiType::NiBillboardNode>},
        {"NiSortAdjustNode", &construct<NiSortAdjustNode, NiType::NiSortAdjustNode>},
        {"NiPalette", &construct<NiPalette, NiType::NiPalette>},
        {"NiPixelData", &construct<NiPixelData, NiType::NiPixelData>},
        {"NiTexture", &construct<NiTexture, NiType::NiTexture>},
        {"NiSourceTexture", &construct<NiSourceTexture, NiType::NiSourceTexture>},
        {"NiDynamicEffect", &construct<NiDynamicEffect, NiType::NiDynamicEffect>},
        {"NiTextureEffect", &construct<NiTextureEffect, NiType::NiTextureEffect>},
        {"NiLight", &construct<NiLight, NiType::NiLight>},
        {"NiAmbientLight", &construct<NiAmbientLight, NiType::NiAmbientLight>},
        {"NiDirectionalLight", &construct<NiDirectionalLight, NiType::NiDirectionalLight>},
        {"NiPointLight", &construct<NiPointLight, NiType::NiPointLight>},
        {"NiSpotLight", &construct<NiSpotLight, NiType::NiSpotLight>},
        {"NiTimeController", &construct<NiTimeController, NiType::NiTimeController>},
        {"NiEmitterModifier", &construct<NiEmitterModifier, NiType::NiEmitterModifier>},
        {"NiParticleModifier", &construct<NiParticleModifier, NiType::NiParticleModifier>},
        {"NiParticleGrowFade", &construct<NiParticleGrowFade, NiType::NiParticleGrowFade>},
        {"NiParticleCollider", &construct<NiParticleCollider, NiType::NiParticleCollider>},
        {"NiParticleSystemController",
         &construct<NiParticleSystemController, NiType::NiParticleSystemController>},
        {"NiUVData", &construct<NiUVData, NiType::NiUVData>},
        {"NiUVController", &construct<NiUVController, NiType::NiUVController>},
        {"NiMorphData", &construct<NiMorphData, NiType::NiMorphData>},
        {"NiInterpController", &construct<NiInterpController, NiType::NiInterpController>},
        {"NiGeomMorpherController",
         &construct<NiGeomMorpherController, NiType::NiGeomMorpherController>},
        {"NiSingleInterpController",
         &construct<NiFloatInterpController, NiType::NiSingleInterpController>},
        {"NiFloatInterpController",
         &construct<NiFloatInterpController, NiType::NiFloatInterpController>},
        {"NiFlipController", &construct<NiFlipController, NiType::NiFlipController>},
        {"NiExtraData", &construct<NiExtraData, NiType::NiExtraData>},
        {"NiStringExtraData", &construct<NiStringExtraData, NiType::NiStringExtraData>},
        {"NiGeometry", &construct<NiGeometry, NiType::NiGeometry>},
        {"NiGeometryData", &construct<NiGeometryData, NiType::NiGeometryData>},
        {"NiTriBasedGeom", &construct<NiTriBasedGeom, NiType::NiTriBasedGeom>},
        {"NiTriBasedGeomData", &construct<NiTriBasedGeomData, NiType::NiTriBasedGeomData>},
        {"NiTriShape", &construct<NiTriShape, NiType::NiTriShape>},
        {"NiTriShapeData", &construct<NiTriShapeData, NiType::NiTriShapeData>},
        {"NiParticles", &construct<NiParticles, NiType::NiParticles>},
        {"NiParticlesData", &construct<NiParticlesData, NiType::NiParticlesData>},
        {"NiAutoNormalParticles", &construct<NiAutoNormalParticles, NiType::NiAutoNormalParticles>},
        {"NiAutoNormalParticlesData",
         &construct<NiAutoNormalParticlesData, NiType::NiAutoNormalParticlesData>},
        {"NiAlphaProperty", &construct<NiAlphaProperty, NiType::NiAlphaProperty>},
        {"NiShaderProperty", &construct<NiShaderProperty, NiType::NiShaderProperty>},
        {"NiSpecularProperty", &construct<NiSpecularProperty, NiType::NiSpecularProperty>},
        {"NiZBufferProperty", &construct<NiZBufferProperty, NiType::NiZBufferProperty>},
        {"NiDitherProperty", &construct<NiDitherProperty, NiType::NiDitherProperty>},
        {"NiFogProperty", &construct<NiFogProperty, NiType::NiFogProperty>},
        {"NiWireframeProperty", &construct<NiWireframeProperty, NiType::NiWireframeProperty>},
        {"NiMaterialProperty", &construct<NiMaterialProperty, NiType::NiMaterialProperty>},
        {"NiVertexColorProperty", &construct<NiVertexColorProperty, NiType::NiVertexColorProperty>},
        {"NiTexturingProperty", &construct<NiTexturingProperty, NiType::NiTexturingProperty>},
        {"NiSkinData", &construct<NiSkinData, NiType::NiSkinData>},
        {"NiSkinInstance", &construct<NiSkinInstance, NiType::NiSkinInstance>},
    };
}

static const std::map<std::string, CreateBlock> factory = makeFactory();

static uint32_t getVersion(uint8_t major, uint8_t minor, uint8_t patch, uint8_t revision)
{
    return (major << 24) + (minor << 16) + (patch << 8) + revision;
}

NifReader::NifReader(NifFile &file)
    : m_Path(file.path), m_Version(file.version), m_Blocks(file.blocks), m_Roots(file.roots)
{
}

void NifReader::parse(std::unique_ptr<std::istream> stream)
{
    NifStream nif(*this, std::move(stream));

    // Check header string
    std::string header = nif.getVersionString();
    const std::string expected_header = "NetImmerse File Format";

    const bool is_header_valid = header.compare(0, expected_header.size(), expected_header) == 0;
    if (!is_header_valid)
    {
        throw std::runtime_error("Invalid NIF Header: " + header);
    }

    // Get version
    uint32_t ver = nif.getUint32();

    int major = (ver >> 24) & 0xFF;
    int minor = (ver >> 16) & 0xFF;
    int patch = (ver >> 8) & 0xFF;
    int rev = ver & 0xFF;
    LOG_INFO("NIF Version: {}.{}.{}.{}", major, minor, patch, rev);

    const bool is_version_supported =
        ver == getVersion(3, 3, 0, 13) || ver == getVersion(3, 3, 0, 11);
    if (!is_version_supported)
    {
        LOG_ERROR("Unsupported NIF version: {}", ver);
        throw;
    }
    // Get number of blocks
    const size_t n_blocks = nif.getUint32();
    LOG_INFO("nBlocks: {}", n_blocks);
    m_Blocks.resize(n_blocks);

    // Parse nif file
    for (size_t i = 0; i < n_blocks; i++)
    {
        std::unique_ptr<NiObject> block;

        // Check for block type
        std::string block_type = nif.getString();
        if (block_type.empty())
        {
            LOG_ERROR("Block number {} out of {} is empty!", i, n_blocks);
            throw;
        }

        // Check for block in supported blocks list
        const auto entry = factory.find(block_type);
        if (entry == factory.end())
        {
            LOG_ERROR("Unknown block type {} in file {}", block_type.c_str(),
                      m_Path.string().c_str());
            throw;
        }

        block = entry->second();

        if (!block)
        {
            LOG_ERROR("Block of type {} in file {} was null!", block_type.c_str(),
                      m_Path.string().c_str());
            throw;
        }

        if (block->type == NiType::Unknown)
        {
            LOG_ERROR("Read error with block type {} in file {}", block_type.c_str(),
                      m_Path.string().c_str());
            throw;
        }

        block->typeName = block_type;
        block->index = i;
        block->read(&nif);
        m_Blocks[i] = std::move(block);
    }

    // Get roots
    const size_t n_roots = nif.getUint32();
    LOG_INFO("nRoots: {}", n_roots);
    m_Roots.resize(n_roots);

    for (size_t i = 0; i < n_roots; i++)
    {
        int idx = nif.getInt32();
        if (idx >= 0 && static_cast<size_t>(idx) < m_Blocks.size())
        {
            m_Roots[i] = m_Blocks[idx].get();
        }
        else
        {
            m_Roots[i] = nullptr;
            LOG_WARN("Root #{} was null!", i);
        }
    }

    LOG_INFO("Post processing...");

    for (const auto &block : m_Blocks)
    {
        block->post(*this);
    }
}

} // namespace nif
