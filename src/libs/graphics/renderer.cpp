#include "renderer.hpp"
#include "libs/common/log.hpp"

namespace graphics
{

void Renderer::init()
{
    auto vertexShader = std::make_unique<Shader>(ShaderType::VERTEX);
    vertexShader->setSource("assets/shaders/default.vert");
    vertexShader->compile();

    auto fragmentShader = std::make_unique<Shader>(ShaderType::FRAGMENT);
    fragmentShader->setSource("assets/shaders/default.frag");
    fragmentShader->compile();

    auto defaultShader = std::make_unique<ShaderProgram>();
    defaultShader->attach(vertexShader.get());
    defaultShader->attach(fragmentShader.get());
    defaultShader->link();

    _shaders["default"] = std::move(defaultShader);
}

void Renderer::close()
{
}

void Renderer::preDraw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // NOTE: Faces in gamebryo are flipped
    // glCullFace(GL_FRONT);
    // glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Camera *camera)
{
    auto defaultShader = _shaders["default"].get();
    defaultShader->use();

    glm::mat4 model(1.0f);
    defaultShader->setUniform("uModel", model);
    defaultShader->setUniform("uModelInv", glm::inverse(model));
    defaultShader->setUniform("uView", camera->getViewMatrix());
    defaultShader->setUniform("uProjection", camera->getProjectionMatrix());
    defaultShader->setUniform("uViewPos", camera->getPosition());
}

void Renderer::postDraw()
{
}

} // namespace graphics