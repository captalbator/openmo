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
    Renderer() {}
    ~Renderer() {}

    void init(int viewWidth, int viewHeight);
    void close();

    unsigned int pick(Camera *camera, Scene *scene, int x, int y);
    void draw(Camera *camera, Scene *scene);

    void updateViewportSize(int w, int h);
    glm::vec2 getViewportSize() const { return _viewportSize; }

    const GLuint getRenderTexture() const { return _finalTexture; }

private:
    glm::vec2 _viewportSize{};

    std::unique_ptr<ShaderProgram> _solidShader;
    std::unique_ptr<ShaderProgram> _transparentShader;
    std::unique_ptr<ShaderProgram> _compositeShader;
    std::unique_ptr<ShaderProgram> _screenShader;
    std::unique_ptr<ShaderProgram> _pickingShader;

    GLuint _opaqueFBO;
    GLuint _transparentFBO;
    GLuint _pickingFBO;
    GLuint _finalFBO;

    GLuint _opaqueTexture;
    GLuint _pickingTexture;
    GLuint _depthTexture;
    GLuint _accumTexture;
    GLuint _revealTexture;

    GLuint _finalTexture;

    std::unique_ptr<Mesh> _quadMesh;
};

} // namespace graphics