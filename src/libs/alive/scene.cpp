#include "scene.hpp"

#include "libs/common/log.hpp"
#include "libs/nif/geom.hpp"
#include "libs/nif/shaders.hpp"

graphics::Texture *
Scene::getSceneTextureFromTexturingProperty(nif::NiTexturingProperty *texturingProperty)
{
  graphics::Texture *ret;

  auto possibleTexture = _sceneTextures.find(texturingProperty->index);
  if (possibleTexture != _sceneTextures.end())
  {
    ret = possibleTexture->second.get();
  }
  else
  {
    nif::TexDesc &texDesc = texturingProperty->textures[0];

    auto texture = std::make_unique<graphics::Texture>();

    switch (texDesc.clampMode)
    {
    case nif::CLAMP_S_CLAMP_T:
      texture->setWrapMode(graphics::Texture::WrapMode::CLAMP_TO_EDGE);
      break;
    default:
      break;
    }

    switch (texDesc.filterMode)
    {
    case nif::FILTER_NEAREST:
      texture->setFilterMode(graphics::Texture::FilterMode::NEAREST,
                             graphics::Texture::FilterMode::NEAREST);
      break;
    case nif::FILTER_BILERP:
      texture->setFilterMode(graphics::Texture::FilterMode::LINEAR,
                             graphics::Texture::FilterMode::LINEAR);
      break;
    case nif::FILTER_TRILERP:
      texture->setFilterMode(graphics::Texture::FilterMode::LINEAR_MIPMAP_LINEAR,
                             graphics::Texture::FilterMode::LINEAR);
      break;
    case nif::FILTER_NEAREST_MIPNEAREST:
      texture->setFilterMode(graphics::Texture::FilterMode::NEAREST_MIPMAP_NEAREST,
                             graphics::Texture::FilterMode::NEAREST);
      break;
    case nif::FILTER_NEAREST_MIPLERP:
      texture->setFilterMode(graphics::Texture::FilterMode::NEAREST_MIPMAP_LINEAR,
                             graphics::Texture::FilterMode::NEAREST);
      break;
    case nif::FILTER_BILERP_MIPNEAREST:
      texture->setFilterMode(graphics::Texture::FilterMode::NEAREST_MIPMAP_NEAREST,
                             graphics::Texture::FilterMode::LINEAR);
      break;
    default:
      break;
    }

    nif::NiSourceTexture *sourceTexture = texDesc.textureSource.ptr();
    nif::NiPixelData *pixelData = sourceTexture->textureData.ptr();

    graphics::Texture::Format format = graphics::Texture::Format::RGBA8;
    switch (pixelData->pixelFormat)
    {
    case nif::FMT_RGB:
      format = graphics::Texture::Format::RGB8;
      break;
    case nif::FMT_RGBA:
      format = graphics::Texture::Format::RGBA8;
      break;
    case nif::FMT_DXT1:
    case nif::FMT_DXT3:
      format = graphics::Texture::Format::DXT1;
      break;
    case nif::FMT_DXT5:
      format = graphics::Texture::Format::DXT5;
      break;
    default:
      LOG_WARN("unknown texture format {}", (uint32_t)pixelData->pixelFormat);
      break;
    }

    texture->configure();

    auto mipmap = pixelData->mipmaps[0];

    texture->setPixels(mipmap.width, mipmap.height, format, pixelData->data, true);
    texture->generateMipmap();

    _sceneTextures.insert(std::make_pair(texturingProperty->index, std::move(texture)));

    auto possibleTexture = _sceneTextures.find(texturingProperty->index);
    ret = possibleTexture->second.get();
  }

  return ret;
}

void Scene::loadRegion(std::filesystem::path path)
{
  _xrg = std::make_unique<alive::XrgFile>(path);
  _xrg->parse();

  std::filesystem::path xinPath = _xrg->xinPath;
  path.replace_filename(xinPath.filename());

  _xin = std::make_unique<alive::XinFile>(path);
  _xin->parse();

  path.replace_filename(_xin->zoneInfo.sceneGraph);
  _sceneGraph = std::make_unique<nif::NifFile>(path);

  auto fstream = std::fstream(path, std::ios_base::in | std::ios_base::binary);
  auto stream = std::make_unique<std::istream>(fstream.rdbuf());

  nif::NifReader reader(*_sceneGraph);
  reader.parse(std::move(stream));

  // Load scene meshes
  for (const auto &block : _sceneGraph->blocks)
  {
    if (block->typeName == "NiTriShape")
    {
      auto triShape = static_cast<nif::NiTriShape *>(block.get());
      if (triShape->data.empty())
      {
        continue;
      }

      graphics::Texture *meshTexture = nullptr;

      bool hasAlpha = false;
      for (const auto &property : triShape->properties)
      {
        auto ptr = property.ptr();
        if (ptr->typeName == "NiTexturingProperty")
        {
          meshTexture =
              getSceneTextureFromTexturingProperty(static_cast<nif::NiTexturingProperty *>(ptr));
        }

        if (ptr->typeName == "NiAlphaProperty")
        {
          hasAlpha = true;
        }
      }

      nif::NiTriShapeData *triShapeData = static_cast<nif::NiTriShapeData *>(triShape->data.ptr());
      auto mesh = std::make_unique<graphics::Mesh>();

      graphics::VertexLayout layout;
      layout.addElement(0, 3, graphics::VertexType::FLOAT);
      if (triShapeData->hasNormals)
      {
        layout.addElement(1, 3, graphics::VertexType::FLOAT);
      }

      if (triShapeData->hasUVs)
      {
        layout.addElement(2, 2, graphics::VertexType::FLOAT);
      }

      std::vector<float> vertices;

      for (size_t i = 0; i < triShapeData->vertices.size(); i++)
      {
        auto vert = triShapeData->vertices[i];
        vertices.push_back(vert.x);
        vertices.push_back(vert.y);
        vertices.push_back(vert.z);

        if (triShapeData->hasNormals)
        {
          auto normals = triShapeData->normals[i];
          vertices.push_back(normals.x);
          vertices.push_back(normals.y);
          vertices.push_back(normals.z);
        }

        if (meshTexture != nullptr)
        {
          auto uv = triShapeData->uvSets[0][i];
          vertices.push_back(uv.x);
          vertices.push_back(uv.y);
        }
      }

      mesh->setTexture(meshTexture);
      mesh->setVertices(std::move(vertices));
      mesh->setIndices(triShapeData->triangles);

      mesh->setLayout(layout);

      Object obj = {};
      obj.isNif = true;
      obj.niObject = block.get();
      obj.mesh = mesh.get();

      _sceneObjects.push_back(std::move(obj));

      if (triShapeData->hasNormals)
      {
        if (hasAlpha)
        {
          _alphaMeshes.push_back(std::move(mesh));
        }
        else
        {
          _opaqueMeshes.push_back(std::move(mesh));
        }
      }
      else
      {
        _navMeshes.push_back(std::move(mesh));
      }
    }
  }

  _isLoaded = true;
}

void Scene::clear()
{
  _sceneGraph.reset(nullptr);
  _xin.reset(nullptr);
  _xrg.reset(nullptr);

  _isLoaded = false;
}

void Scene::drawAll()
{
  if (!_xin)
    return;

  for (const auto &obj : _sceneObjects)
  {
    obj.mesh->draw();
  }
}

void Scene::drawOpaque()
{
  if (!_xin)
    return;

  for (const auto &mesh : _opaqueMeshes)
  {
    mesh->draw();
  }
}

void Scene::drawTransparent()
{
  if (!_xin)
    return;

  // TODO: Sort back to front

  for (const auto &mesh : _alphaMeshes)
  {
    mesh->draw();
  }
}

void Scene::drawNavMeshes()
{
  if (!_xin)
    return;

  for (const auto &mesh : _navMeshes)
  {
    mesh->draw();
  }
}

void Scene::selectObject(int objectId)
{
  LOG_INFO("Selected object id {}", objectId);
  _selectedObject = objectId;
}
