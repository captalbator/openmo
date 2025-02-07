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
  void handleWindowEvent(SDL_WindowEvent &event);

private:
  float drawMenuBar();
  void drawSceneView();
  void drawNifExplorer();
  void drawSceneObject(int objIndex, Object *obj);
  // void drawTagExplorer();
  // void drawProperties();

  EditorOptions _options;

  struct
  {
    bool isDown{false};
    int x{0};
    int y{0};
  } _pointerState;

  bool appFocused;

  std::unique_ptr<graphics::Window> _window;
  std::unique_ptr<graphics::Renderer> _renderer;
  std::unique_ptr<Scene> _scene;
  std::unique_ptr<graphics::Camera> _camera;
};
