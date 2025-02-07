#pragma once

#include "camera.hpp"
#include "libs/alive/scene.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace graphics
{

class Renderer
{
public:
  Renderer(float clipNear, float clipFar) : _near(clipNear), _far(clipFar) {}
  ~Renderer() {}

  void init(int viewWidth, int viewHeight);
  void close();

  unsigned int pick(Camera *camera, Scene *scene, int x, int y);
  void draw(Camera *camera, Scene *scene);

  void updateViewportSize(int w, int h);
  glm::vec2 getViewportSize() const { return _viewportSize; }

  GLuint getRenderTexture() const { return _finalTexture; }

private:
  float _near;
  float _far;

  glm::vec2 _viewportSize{};

  std::unique_ptr<ShaderProgram> _screenShader;
  std::unique_ptr<ShaderProgram> _solidShader;
  std::unique_ptr<ShaderProgram> _transparentShader;
  std::unique_ptr<ShaderProgram> _pickingShader;

  // PICKING
  GLuint _pickingFBO;
  GLuint _pickingTexture;

  // FINAL IMAGE
  GLuint _finalFBO;
  GLuint _finalTexture;

  GLuint _depthBuffer;

  std::unique_ptr<Mesh> _quadMesh;
};

} // namespace graphics
