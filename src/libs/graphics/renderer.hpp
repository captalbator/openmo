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

    void init(int scrWidth, int scrHeight);
    void close();

    void draw(Camera *camera, Scene *scene);

private:
    std::unique_ptr<ShaderProgram> _solidShader;
    std::unique_ptr<ShaderProgram> _transparentShader;
    std::unique_ptr<ShaderProgram> _compositeShader;
    std::unique_ptr<ShaderProgram> _screenShader;

    GLuint _opaqueFBO;
    GLuint _transparentFBO;

    GLuint _opaqueTexture;
    GLuint _depthTexture;
    GLuint _accumTexture;
    GLuint _revealTexture;

    std::unique_ptr<Mesh> _quadMesh;
};

} // namespace graphics