#include "editor.hpp"

#include "libs/common/log.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/shader.hpp"
#include "libs/nif/geom.hpp"
#include "libs/nif/nodes.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_internal.h"

Editor::Editor(EditorOptions opts) : _options(std::move(opts))
{
  float nearPlane = 5.0f;
  float farPlane = 10000.0f;
  _camera = std::make_unique<graphics::Camera>(
      glm::vec3{.0f, .0f, .0f}, 90.0f, opts.windowOpts.w / opts.windowOpts.h, nearPlane, farPlane);

  _window = std::make_unique<graphics::Window>();
  _renderer = std::make_unique<graphics::Renderer>(nearPlane, farPlane);
  _scene = std::make_unique<Scene>();
}

Editor::~Editor()
{
  _window->close();
  _renderer->close();

  SDL_Quit();
}

static void setupLayout(ImGuiID dockspaceId, float menuBarHeight)
{
  //ImGui::DockBuilderRemoveNode(dockspaceId);
  ImGui::DockBuilderAddNode(dockspaceId);
  ImGui::DockBuilderSetNodeSize(
      dockspaceId, ImVec2{ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - menuBarHeight});

  ImGuiID dock_main_id = dockspaceId;
  ImGuiID dock_id_explorer =
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
  ImGuiID dock_id_actors =
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
  ImGuiID dock_id_properties =
      ImGui::DockBuilderSplitNode(dock_id_explorer, ImGuiDir_Down, 0.50f, NULL, &dock_id_explorer);
  ImGuiID dock_id_scene =
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 1.0f, NULL, &dock_main_id);

  ImGui::DockBuilderDockWindow("Explorer", dock_id_explorer);
  ImGui::DockBuilderDockWindow("Actor List", dock_id_actors);
  ImGui::DockBuilderDockWindow("Properties", dock_id_properties);
  ImGui::DockBuilderDockWindow("Scene", dock_id_scene);

  ImGui::DockBuilderFinish(dockspaceId);
}

float Editor::drawMenuBar()
{
  float barHeight = 0.0f;

  bool shouldOpenAbout = false;

  if (ImGui::BeginMainMenuBar())
  {
    barHeight = ImGui::GetWindowHeight();

    if (ImGui::BeginMenu("File"))
    {
      ImGui::MenuItem("New", "CTRL+N", false, false);

      if (ImGui::MenuItem("Open", "CTRL+O"))
      {
      }

      if (ImGui::MenuItem("Save", "CTRL+S", false, false))
      {
      }

      if (ImGui::MenuItem("Close", NULL, false, false))
      {
      }

      if (ImGui::MenuItem("Exit", NULL, false, false))
      {
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit", false))
    {
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View", false))
    {
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("About"))
      {
        shouldOpenAbout = true;
      }
      ImGui::EndMenu();
    }

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("FPS XXXX").x);
    ImGui::Text("%1.f FPS", ImGui::GetIO().Framerate);

    ImGui::EndMainMenuBar();
  }

  if (shouldOpenAbout)
  {
    ImGui::OpenPopup("About");
  }

  auto center = ImGui::GetMainViewport()->GetCenter();

  bool aboutOpen = true;
  ImGui::SetNextWindowSize(ImVec2(400, 100), 0);
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("About", &aboutOpen,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize))
  {
    ImGui::Text("Gabbit Editor v0.1.0");
    // TODO: Add licensing and other links
    //       a bit like how imhex does it
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f);
    if (ImGui::Button("Close"))
      ImGui::CloseCurrentPopup();
    ImGui::EndPopup();
  }

  return barHeight;
}

void Editor::drawSceneView()
{
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavFocus |
                                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  ImGui::Begin("Scene");

  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 windowPos = ImGui::GetWindowPos();

  glm::vec2 renderSize = _renderer->getViewportSize();
  if (renderSize.x != windowSize.x || renderSize.y != windowSize.y)
  {
    _camera->setViewportSize(windowSize.x, windowSize.y);
    _renderer->updateViewportSize(windowSize.x, windowSize.y);
  }

	_renderer->draw(_camera.get(), _scene.get());

  ImGui::GetWindowDrawList()->AddImage((void *)(intptr_t)_renderer->getRenderTexture(), windowPos,
                                       {windowPos.x + windowSize.x, windowPos.y + windowSize.y}, { 0, 1 }, { 1, 0 });

  ImGui::End();
  ImGui::PopStyleVar();
}

void Editor::drawSceneObject(int objIndex, Object *obj)
{
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

  bool hasLeaves = false;
  bool opened = false;

  if (obj->isNif)
  {
    auto block = obj->niObject;

    // This sucks, but it's the only way to individually manage the casts.
    // TODO: Figure out a better way of doing this?
    auto nodeCast = dynamic_cast<nif::NiNode *>(block);
    auto objectNetCast = dynamic_cast<nif::NiObjectNET *>(block);
    auto avObjectCast = dynamic_cast<nif::NiAVObject *>(block);
    auto geomCast = dynamic_cast<nif::NiGeometry *>(block);

    if (nodeCast != nullptr)
      if (nodeCast->children.size() > 0)
        hasLeaves = true;

    if (objectNetCast != nullptr)
    {
      if (!objectNetCast->extraData.empty())
        hasLeaves = true;

      if (!objectNetCast->controller.empty())
        hasLeaves = true;
    }

    if (avObjectCast != nullptr)
      if (avObjectCast->properties.size() > 0)
        hasLeaves = true;

    if (geomCast != nullptr)
    {
      if (!geomCast->data.empty())
        hasLeaves = true;

      if (!geomCast->skinInstance.empty())
        hasLeaves = true;
    }

    if (hasLeaves)
      flags = ImGuiTreeNodeFlags_OpenOnArrow;

    flags |= ((_scene->getSelectedObject() == objIndex) ? ImGuiTreeNodeFlags_Selected : 0);

    if (objectNetCast != nullptr && !objectNetCast->name.empty())
    {
      opened =
          ImGui::TreeNodeEx((void *)(intptr_t)block->index, flags, "%d - %s - %s", block->index,
                            block->typeName.c_str(), objectNetCast->name.c_str());
    }
    else
    {
      opened = ImGui::TreeNodeEx((void *)(intptr_t)block->index, flags, "%d - %s", block->index,
                                 block->typeName.c_str());
    }

    if (opened && (flags & ImGuiTreeNodeFlags_Leaf) == 0)
    {
      if (objectNetCast != nullptr)
      {
        if (!objectNetCast->extraData.empty())
        {
          drawSceneObject(objectNetCast->extraData.index(),
                          &_scene->getSceneObjects()[objectNetCast->extraData.index()]);
        }
      }

      ImGui::TreePop();
    }

    if (ImGui::IsItemClicked())
    {
      _scene->selectObject(objIndex);
    }

    if ((flags & ImGuiTreeNodeFlags_Leaf) != 0)
    {
      ImGui::TreePop();
    }
  }
}

void Editor::drawNifExplorer()
{
  ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
  ImGui::Begin("Explorer", nullptr, flags);

  if (!_scene->isLoaded())
  {
    ImGui::End();
    return;
  }

  auto objects = _scene->getSceneObjects();
  for (int index = 0; index < objects.size(); index++)
  {
    auto obj = objects[index];
    if (obj.isNif)
      drawSceneObject(index, &obj);
  }

  ImGui::End();
}

void Editor::run()
{
  _window->init(_options.windowOpts);
  _renderer->init(_options.windowOpts.w, _options.windowOpts.h);

  // init imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplSDL2_InitForOpenGL(_window->getWindow(), _window->getContext());
  ImGui_ImplOpenGL3_Init("#version 400 core");

  if (!_options.openFile.empty())
  {
    _scene->loadRegion(_options.openFile);
  }

  uint64_t currentTick = SDL_GetPerformanceCounter();
  uint64_t lastTick = 0;

  double deltaTime = 0;

  bool running = true;
  while (running)
  {
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();

    deltaTime = ((currentTick - lastTick) * 1000 / (double)SDL_GetPerformanceFrequency());

    SDL_Event e;

    while (!appFocused ? SDL_WaitEvent(&e) : SDL_PollEvent(&e))
    {
      ImGui_ImplSDL2_ProcessEvent(&e);
      switch (e.type)
      {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_WINDOWEVENT:
        handleWindowEvent(e.window);
        //_window->setDimensions(e.window.data1, e.window.data2);
        break;
      case SDL_MOUSEMOTION:
        handleMouseMotionEvent(e.motion);
        break;
      case SDL_MOUSEWHEEL:
        handleMouseWheelEvent(e.wheel);
        break;
      case SDL_MOUSEBUTTONDOWN:
        handleMouseClickEvent(e.button);
      default:
        break;
      }
    }

    _camera->processMovement(deltaTime);
    _camera->update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    float menuBarHeight = drawMenuBar();

    ImGuiID dockspaceId = ImGui::GetID("InvisibleWindowDockspace");

    if (ImGui::DockBuilderGetNode(dockspaceId) == NULL)
    {
      setupLayout(dockspaceId, menuBarHeight);
    }

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 pos = viewport->Pos;
    ImVec2 size = viewport->Size;
    pos.y += menuBarHeight;
    size.y -= menuBarHeight;

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f),
                     ImGuiDockNodeFlags_PassthruCentralNode |
                         ImGuiDockNodeFlags_NoWindowMenuButton);

    ImGui::End();

    if (_pointerState.isDown)
    {
      LOG_INFO("MOUSE CLICKED AT {}, {}", _pointerState.x, _pointerState.y);
      _pointerState.isDown = false;

      unsigned int objectId =
          _renderer->pick(_camera.get(), _scene.get(), _pointerState.x, _pointerState.y);
      _scene->selectObject(objectId - 1);
    }


    //drawNifExplorer();
    drawSceneView();


    // if (ImGui::Begin("GabbitDockSpace", nullptr, windowFlags))
    //{
    //   ImGui::PopStyleVar(3);

    //  drawSceneView();
    //  drawNifExplorer();

    //  ImGui::End();
    //}
    // else
    //{
    //  ImGui::PopStyleVar(3);
    //}

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    _window->swap();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void Editor::handleMouseMotionEvent(SDL_MouseMotionEvent &event)
{
  int dx, dy;
  bool held;
  if (SDL_GetRelativeMouseState(&dx, &dy) & SDL_BUTTON(3))
  {
    held = true;
  }
  else
  {
    held = false;
  }

  SDL_SetRelativeMouseMode((SDL_bool)(held));

  _camera->processMouseMovement(dx, dy, held);
}

void Editor::handleMouseWheelEvent(SDL_MouseWheelEvent &event)
{
  _camera->processMovement(event.y);
}

void Editor::handleMouseClickEvent(SDL_MouseButtonEvent &event)
{
  if (event.button == SDL_BUTTON_LEFT)
  {
    _pointerState.isDown = true;
    _pointerState.x = event.x;
    _pointerState.y = event.y;
  }
}

void Editor::handleWindowEvent(SDL_WindowEvent &event)
{
  switch (event.event)
  {
  case SDL_WINDOWEVENT_SHOWN:
    LOG_INFO("WINDOW SHOWN");
    break;
  case SDL_WINDOWEVENT_HIDDEN:
    LOG_INFO("WINDOW HIDDEN");
    break;
  case SDL_WINDOWEVENT_MINIMIZED:
    LOG_INFO("WINDOW MINIMIZED");
    break;
  case SDL_WINDOWEVENT_FOCUS_LOST:
    appFocused = false;
    break;
  case SDL_WINDOWEVENT_FOCUS_GAINED:
			{
				appFocused = true;
				LOG_TRACE("Focused");
			}
    break;
  case SDL_WINDOWEVENT_RESIZED:
    _window->setDimensions(event.data1, event.data2);
    break;
  }
}
