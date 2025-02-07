#pragma once

#include "libs/graphics/mesh.hpp"
#include "libs/nif/nifreader.hpp"

struct Object
{
  bool isActor{false};
  bool isNif{false};

  // Actor *actor;
  nif::NiObject *niObject;

  graphics::Mesh *mesh;
};
