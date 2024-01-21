#include "window.hpp"
#include "libs/common/log.hpp"

namespace graphics
{

void Window::init(WindowOptions options)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    _window = SDL_CreateWindow(options.title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, options.w, options.h,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    LOG_ASSERT(_window != NULL, "Failed to create window: {}", SDL_GetError());

    _context = SDL_GL_CreateContext(_window);
    LOG_ASSERT(_context != NULL, "Failed to create gl context: {}", SDL_GetError());

    int version = gladLoadGLLoader(SDL_GL_GetProcAddress);
    LOG_ASSERT(version != 0, "Failed to load glad");

    LOG_TRACE("GL Vendor: {}", (const char *)glGetString(GL_VENDOR));
    LOG_TRACE("GL Renderer: {}", (const char *)glGetString(GL_RENDERER));
    LOG_TRACE("GL Version: {}", (const char *)glGetString(GL_VERSION));

    SDL_GL_MakeCurrent(_window, _context);
    SDL_GL_SetSwapInterval(1);

    glViewport(0, 0, options.w, options.h);
}

void Window::close()
{
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
}

void Window::setTitle(std::string title)
{
    SDL_SetWindowTitle(_window, title.c_str());
}

void Window::swap()
{
    SDL_GL_SwapWindow(_window);
}

} // namespace graphics