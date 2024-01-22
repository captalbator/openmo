#pragma once

#include "camera.hpp"
#include "shader.hpp"

namespace graphics
{

class Renderer
{
public:
    Renderer() {}
    ~Renderer() {}

    void init();
    void close();

    void preDraw();
    void draw(Camera *camera);
    void postDraw();

private:
    std::map<std::string, std::unique_ptr<ShaderProgram>> _shaders;
};

} // namespace graphics