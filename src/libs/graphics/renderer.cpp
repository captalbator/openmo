#include "renderer.hpp"
#include "libs/common/log.hpp"

namespace graphics
{

static const float quadVertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f};

void Renderer::init(int viewWidth, int viewHeight)
{
  _viewportSize.x = viewWidth;
  _viewportSize.y = viewHeight;

  auto geomVert = std::make_unique<Shader>(ShaderType::VERTEX);
  geomVert->setSource("assets/shaders/geom.vert");
  geomVert->compile();

  auto solidFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
  solidFrag->setSource("assets/shaders/solid.frag");
  solidFrag->compile();

  auto screenVert = std::make_unique<Shader>(ShaderType::VERTEX);
  screenVert->setSource("assets/shaders/screen.vert");
  screenVert->compile();

  auto screenFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
  screenFrag->setSource("assets/shaders/screen.frag");
  screenFrag->compile();

  auto pickingVert = std::make_unique<Shader>(ShaderType::VERTEX);
  pickingVert->setSource("assets/shaders/picking.vert");
  pickingVert->compile();

  auto pickingFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
  pickingFrag->setSource("assets/shaders/picking.frag");
  pickingFrag->compile();

  auto transparentFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
  transparentFrag->setSource("assets/shaders/transparent.frag");
  transparentFrag->compile();

  _transparentShader = std::make_unique<ShaderProgram>();
  _transparentShader->attach(geomVert.get());
  _transparentShader->attach(transparentFrag.get());
  _transparentShader->link();

  _solidShader = std::make_unique<ShaderProgram>();
  _solidShader->attach(geomVert.get());
  _solidShader->attach(solidFrag.get());
  _solidShader->link();

  _pickingShader = std::make_unique<ShaderProgram>();
  _pickingShader->attach(pickingVert.get());
  _pickingShader->attach(pickingFrag.get());
  _pickingShader->link();

  _screenShader = std::make_unique<ShaderProgram>();
  _screenShader->attach(screenVert.get());
  _screenShader->attach(screenFrag.get());

  // Generate render buffer
  glGenRenderbuffers(1, &_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewWidth, viewHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // SETUP PICKING
  glGenFramebuffers(1, &_pickingFBO);
  glGenTextures(1, &_pickingTexture);
  glBindTexture(GL_TEXTURE_2D, _pickingTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, viewWidth, viewHeight, 0, GL_RGB_INTEGER,
               GL_UNSIGNED_INT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, _pickingFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pickingTexture, 0);
  glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                            _depthBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    LOG_ERROR("PICKING FRAMEBUFFER ISN'T COMPLETE!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // SETUP REGULAR RENDERING
  glGenFramebuffers(1, &_finalFBO);
  glGenTextures(1, &_finalTexture);
  glBindTexture(GL_TEXTURE_2D, _finalTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewWidth, viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _finalTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    LOG_ERROR("FINAL FRAMEBUFFER ISN'T COMPLETE!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  _quadMesh = std::make_unique<Mesh>();
  _quadMesh->setVertices(std::vector<float>(std::begin(quadVertices), std::end(quadVertices)));

  VertexLayout quadLayout;
  quadLayout.addElement(0, 3, VertexType::FLOAT);
  quadLayout.addElement(1, 2, VertexType::FLOAT);

  _quadMesh->setLayout(quadLayout);

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void Renderer::close()
{
  const GLuint framebuffers[] = { _pickingFBO, _finalFBO };
  glDeleteFramebuffers(2, framebuffers);

  const GLuint textures[] = { _pickingTexture, _finalTexture };
  glDeleteTextures(2, textures);

  glDeleteRenderbuffers(1, &_depthBuffer);

  _screenShader.release();
  _solidShader.release();
  _transparentShader.release();
  _pickingShader.release();

  _quadMesh.release();
}

void Renderer::updateViewportSize(int w, int h)
{
  LOG_INFO("Updating viewport size");

  _viewportSize.x = w;
  _viewportSize.y = h;

  glBindTexture(GL_TEXTURE_2D, _finalTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glBindTexture(GL_TEXTURE_2D, _pickingTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, w, h, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Renderer::pick(Camera *camera, Scene *scene, int x, int y)
{
  glBindFramebuffer(GL_FRAMEBUFFER, _pickingFBO);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _pickingShader->use();

  _pickingShader->setUniform("uView", camera->getViewMatrix());
  _pickingShader->setUniform("uProjection", camera->getProjectionMatrix());

  auto sceneObjects = scene->getSceneObjects();
  for (unsigned int i = 0; i < sceneObjects.size(); i++)
  {
    const auto &obj = sceneObjects[i];
    _pickingShader->setUniform("uObjectIndex", i + 1);
    obj.mesh->draw();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, _pickingFBO);

  glReadBuffer(GL_COLOR_ATTACHMENT0);

  // read picking texture to get object id
  unsigned int values[4];
  glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &values);

  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

  return values[0];
}

void Renderer::draw(Camera *camera, Scene *scene)
{
  glBindFramebuffer(GL_FRAMEBUFFER, _finalFBO);

  // Draw opaque geometry
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, _viewportSize.x, _viewportSize.y);

  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glActiveTexture(GL_TEXTURE0);

  _solidShader->use();

  _solidShader->setUniform("uView", camera->getViewMatrix());
  _solidShader->setUniform("uProjection", camera->getProjectionMatrix());
  _solidShader->setUniform("uViewPos", camera->getPosition());

  scene->drawOpaque();

  // Transparent pass
  // TODO: Check if we have any transparent objects before doing the transparent pass
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Initialize depth buffer

  _transparentShader->use();
  _transparentShader->setUniform("uView", camera->getViewMatrix());
  _transparentShader->setUniform("uProjection", camera->getProjectionMatrix());
  _transparentShader->setUniform("uViewPos", camera->getPosition());
  scene->drawTransparent();

  // Draw to screen
  _screenShader->use();
  _quadMesh->draw();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace graphics
