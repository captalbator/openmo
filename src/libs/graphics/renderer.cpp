#include "renderer.hpp"
#include "libs/common/log.hpp"

namespace graphics
{

static const glm::mat4 model(1.0f);
static const glm::vec4 zeroFillerVec(0.0f);
static const glm::vec4 oneFillerVec(1.0f);

static const float quadVertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f};

void Renderer::init(int scrWidth, int scrHeight)
{
    auto geomVert = std::make_unique<Shader>(ShaderType::VERTEX);
    geomVert->setSource("assets/shaders/geom.vert");
    geomVert->compile();

    auto solidFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
    solidFrag->setSource("assets/shaders/solid.frag");
    solidFrag->compile();

    auto transparentFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
    transparentFrag->setSource("assets/shaders/transparent.frag");
    transparentFrag->compile();

    auto compositeVert = std::make_unique<Shader>(ShaderType::VERTEX);
    compositeVert->setSource("assets/shaders/composite.vert");
    compositeVert->compile();

    auto compositeFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
    compositeFrag->setSource("assets/shaders/composite.frag");
    compositeFrag->compile();

    auto screenVert = std::make_unique<Shader>(ShaderType::VERTEX);
    screenVert->setSource("assets/shaders/screen.vert");
    screenVert->compile();

    auto screenFrag = std::make_unique<Shader>(ShaderType::FRAGMENT);
    screenFrag->setSource("assets/shaders/screen.frag");
    screenFrag->compile();

    _solidShader = std::make_unique<ShaderProgram>();
    _solidShader->attach(geomVert.get());
    _solidShader->attach(solidFrag.get());
    _solidShader->link();

    _transparentShader = std::make_unique<ShaderProgram>();
    _transparentShader->attach(geomVert.get());
    _transparentShader->attach(transparentFrag.get());
    _transparentShader->link();

    _compositeShader = std::make_unique<ShaderProgram>();
    _compositeShader->attach(compositeVert.get());
    _compositeShader->attach(compositeFrag.get());
    _compositeShader->link();

    _screenShader = std::make_unique<ShaderProgram>();
    _screenShader->attach(screenVert.get());
    _screenShader->attach(screenFrag.get());
    _screenShader->link();

    glGenFramebuffers(1, &_opaqueFBO);
    glGenFramebuffers(1, &_transparentFBO);

    // Setup opaque texture
    glGenTextures(1, &_opaqueTexture);
    glBindTexture(GL_TEXTURE_2D, _opaqueTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_HALF_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Setup depth texture
    glGenTextures(1, &_depthTexture);
    glBindTexture(GL_TEXTURE_2D, _depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, scrWidth, scrHeight, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, _opaqueFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _opaqueTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("OPAQUE FRAMEBUFFER ISN'T COMPLETE!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup accum & reveal for transparent fbo
    glGenTextures(1, &_accumTexture);
    glBindTexture(GL_TEXTURE_2D, _accumTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_HALF_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &_revealTexture);
    glBindTexture(GL_TEXTURE_2D, _revealTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, scrWidth, scrHeight, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, _transparentFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _accumTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _revealTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

    // Tell OpenGL that our framebuffer has 2 draw buffers
    const GLenum transparentDrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, transparentDrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("TRANSPARENT FRAMEBUFFER ISN'T COMPLETE!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _quadMesh = std::make_unique<Mesh>();
    _quadMesh->setVertices(std::vector<float>(std::begin(quadVertices), std::end(quadVertices)));

    VertexLayout quadLayout;
    quadLayout.addElement(0, 3, VertexType::FLOAT);
    quadLayout.addElement(1, 2, VertexType::FLOAT);

    _quadMesh->setLayout(quadLayout);
}

void Renderer::close()
{
}

void Renderer::draw(Camera *camera, Scene *scene)
{
    // Opaque pass
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindFramebuffer(GL_FRAMEBUFFER, _opaqueFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _solidShader->use();

    _solidShader->setUniform("uModel", model);
    _solidShader->setUniform("uModelInv", glm::inverse(model));
    _solidShader->setUniform("uView", camera->getViewMatrix());
    _solidShader->setUniform("uProjection", camera->getProjectionMatrix());
    _solidShader->setUniform("uViewPos", camera->getPosition());

    scene->drawOpaque();

    // Transparent pass
    glBindFramebuffer(GL_FRAMEBUFFER, _transparentFBO);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);

    // Clear our render buffers
    glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
    glBlendEquationi(0, GL_FUNC_ADD);
    glBlendFunci(0, GL_ONE, GL_ONE);

    glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
    glBlendEquationi(1, GL_FUNC_ADD);
    glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

    _transparentShader->use();

    _transparentShader->setUniform("uModel", model);
    _transparentShader->setUniform("uModelInv", glm::inverse(model));
    _transparentShader->setUniform("uView", camera->getViewMatrix());
    _transparentShader->setUniform("uProjection", camera->getProjectionMatrix());

    scene->drawTransparent();

    // Composite pass
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, _opaqueFBO);

    _compositeShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _accumTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _revealTexture);

    _quadMesh->draw();

    // Render to screen
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // Bind backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    _screenShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _opaqueTexture);

    _quadMesh->draw();
}

} // namespace graphics