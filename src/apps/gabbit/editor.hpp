#pragma once

#include "libs/alive/scene.hpp"
#include "libs/graphics/camera.hpp"
#include "libs/graphics/renderer.hpp"
#include "libs/graphics/window.hpp"

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
    void handleMouseClickEvent(SDL_MouseButtonEvent &event);

private:
    EditorOptions _options;

    struct
    {
        bool isDown{false};
        int x{0};
        int y{0};
    } _pointerState;

    std::unique_ptr<graphics::Window> _window;
    std::unique_ptr<graphics::Renderer> _renderer;
    std::unique_ptr<Scene> _scene;
    std::unique_ptr<graphics::Camera> _camera;
};