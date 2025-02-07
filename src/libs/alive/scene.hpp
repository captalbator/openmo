#pragma once

#include "libs/alive/xinfile.hpp"
#include "libs/alive/xrgfile.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/texture.hpp"
#include "libs/nif/nifreader.hpp"
#include "libs/nif/shaders.hpp"
#include "object.hpp"

class Scene
{
public:
  Scene() {}
  ~Scene() {}

  void loadRegion(std::filesystem::path path);
  void clear();

  void drawAll();
  void drawOpaque();
  void drawTransparent();
  void drawNavMeshes();

  void selectObject(int objectId);
  int getSelectedObject() const { return _selectedObject; }

  std::vector<Object> getSceneObjects() const { return _sceneObjects; }

  bool isLoaded() const { return _isLoaded; }
  nif::NifFile *getSceneGraph() const { return _sceneGraph.get(); }

private:
  bool _isLoaded{false};
  graphics::Texture *getSceneTextureFromTexturingProperty(nif::NiTexturingProperty *ptr);

  std::unique_ptr<alive::XinFile> _xin;
  std::unique_ptr<alive::XrgFile> _xrg;

  std::unique_ptr<nif::NifFile> _sceneGraph;

  std::map<uint32_t, std::unique_ptr<graphics::Texture>> _sceneTextures;

  std::vector<std::unique_ptr<graphics::Mesh>> _opaqueMeshes;
  std::vector<std::unique_ptr<graphics::Mesh>> _alphaMeshes;
  std::vector<std::unique_ptr<graphics::Mesh>> _navMeshes;

  std::vector<Object> _sceneObjects;
  // -1 means no object is selected
  int _selectedObject{-1};
};
