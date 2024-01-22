#pragma once

#include "libs/graphics/camera.hpp"
#include "libs/graphics/renderer.hpp"
#include "libs/graphics/window.hpp"
#include "scene.hpp"

struct EditorOptions
{
    graphics::WindowOptions windowOpts;

    std::filesystem::path openFile;
};

class Editor
{
public:
    Editor(EditorOptions opts);
    ~Editor();

    void createNewRegion(std::filesystem::path path);
    void loadRegion(std::filesystem::path path);

    void run();

    void handleMouseMotionEvent(SDL_MouseMotionEvent &event);
    void handleMouseWheelEvent(SDL_MouseWheelEvent &event);

private:
    EditorOptions _options;

    std::unique_ptr<graphics::Window> _window;
    std::unique_ptr<graphics::Renderer> _renderer;
    std::unique_ptr<Scene> _scene;
    std::unique_ptr<graphics::Camera> _camera;
};