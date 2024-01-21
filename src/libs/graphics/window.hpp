#pragma once

namespace graphics
{

struct WindowOptions
{
    std::string title;
    float w, h;
};

class Window
{
public:
    Window() {}
    ~Window() {}

    void init(WindowOptions options);
    void close();

    void setTitle(std::string title);

    void swap();

private:
    SDL_Window *_window;
    SDL_GLContext _context;
};

} // namespace graphics