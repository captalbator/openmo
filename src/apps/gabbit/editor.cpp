#include "editor.hpp"

#include "libs/common/log.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/shader.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_internal.h"

Editor::Editor(EditorOptions opts) : _options(std::move(opts))
{
    _camera = std::make_unique<graphics::Camera>(
        glm::vec3{.0f, .0f, .0f}, 90.0f, opts.windowOpts.w / opts.windowOpts.h, 1.0f, 3000.0f);

    _window = std::make_unique<graphics::Window>();
    _renderer = std::make_unique<graphics::Renderer>();
    _scene = std::make_unique<Scene>();
}

Editor::~Editor()
{
    _window->close();
    _renderer->close();

    SDL_Quit();
}

void Editor::drawMenuBar()
{
    float barHeight = 0.0f;

    bool shouldOpenAbout = false;

    if (ImGui::BeginMainMenuBar())
    {
        barHeight = ImGui::GetWindowHeight();

        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New", "CTRL+N", false, false);

            if (ImGui::MenuItem("Open", "CTRL+0"))
            {
                // open
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
}

void Editor::drawSceneView()
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavFocus |
                                   ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Scene"))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 windowPos = ImGui::GetWindowPos();

        glm::vec2 renderSize = _renderer->getViewportSize();
        if (renderSize.x != windowSize.x || renderSize.y != windowSize.y)
        {
            _camera->setViewportSize(windowSize.x, windowSize.y);
            _renderer->updateViewportSize(windowSize.x, windowSize.y);
        }

        ImGui::GetWindowDrawList()->AddImage(
            (void *)(intptr_t)_renderer->getRenderTexture(), windowPos,
            {windowPos.x + windowSize.x, windowPos.y + windowSize.y}, {0, 1}, {1, 0});
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Editor::run()
{
    _window->init(_options.windowOpts);
    _renderer->init(800, 600);

    // init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(_window->getWindow(), _window->getContext());
    ImGui_ImplOpenGL3_Init("#version 400 core");

    if (!_options.openFile.empty())
    {
        _scene->loadRegion(_options.openFile);
    }

    uint64_t currentTick = SDL_GetPerformanceCounter();
    uint64_t lastTick = 0;

    double deltaTime = 0;

    LOG_INFO("running...");

    bool running = true;
    while (running)
    {
        lastTick = currentTick;
        currentTick = SDL_GetPerformanceCounter();

        deltaTime = ((currentTick - lastTick) * 1000 / (double)SDL_GetPerformanceFrequency());

        SDL_Event e;
        while (SDL_PollEvent(&e))
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

        // Don't update if the app isn't focused
        if (!appFocused)
        {
            continue;
        }

        _camera->processMovement(deltaTime);
        _camera->update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(
            ImVec2(viewport->Size.x, viewport->Size.y - ImGui::GetTextLineHeightWithSpacing()));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        if (_pointerState.isDown)
        {
            LOG_INFO("MOUSE CLICKED AT {}, {}", _pointerState.x, _pointerState.y);
            _pointerState.isDown = false;

            unsigned int objectId =
                _renderer->pick(_camera.get(), _scene.get(), _pointerState.x, _pointerState.y);
            _scene->selectObject(objectId - 1);
        }

        _renderer->draw(_camera.get(), _scene.get());

        drawMenuBar();

        if (ImGui::Begin("GabbitDockSpace", nullptr, windowFlags))
        {
            ImGui::PopStyleVar(3);
            drawSceneView();
        }
        else
        {
            ImGui::PopStyleVar(3);
        }
        ImGui::End();

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
        appFocused = true;
        break;
    case SDL_WINDOWEVENT_RESIZED:
        _window->setDimensions(event.data1, event.data2);
        break;
    }
}