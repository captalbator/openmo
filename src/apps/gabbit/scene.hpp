#pragma once

#include "libs/alive/xinfile.hpp"
#include "libs/alive/xrgfile.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/texture.hpp"
#include "libs/nif/nifreader.hpp"

class Scene
{
public:
    Scene() {}
    ~Scene() {}

    void loadRegion(std::filesystem::path path);
    void clear();

    void draw();

private:
    std::unique_ptr<alive::XinFile> _xin;
    std::unique_ptr<alive::XrgFile> _xrg;

    std::unique_ptr<nif::NifFile> _sceneGraph;

    std::map<uint32_t, std::unique_ptr<graphics::Texture>> _sceneTextures;
    std::vector<std::unique_ptr<graphics::Mesh>> _sceneMeshes;
};
