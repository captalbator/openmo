#include "editor.hpp"

#include "libs/common/log.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/shader.hpp"

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

void Editor::run()
{
    _window->init(_options.windowOpts);
    _renderer->init(_options.windowOpts.w, _options.windowOpts.h);

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
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                _window->setDimensions(e.window.data1, e.window.data2);
                break;
            case SDL_MOUSEMOTION:
                handleMouseMotionEvent(e.motion);
                break;
            case SDL_MOUSEWHEEL:
                handleMouseWheelEvent(e.wheel);
                break;
            default:
                break;
            }
        }

        _camera->processMovement(deltaTime);
        _camera->update();

        _renderer->draw(_camera.get(), _scene.get());

        _window->swap();
    }
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

    _camera->processMouseMovement(dx, dy, held);
}

void Editor::handleMouseWheelEvent(SDL_MouseWheelEvent &event)
{
    _camera->processMovement(event.y);
}