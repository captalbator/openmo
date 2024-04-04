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
    void setDimensions(int w, int h);

    void swap();

    SDL_Window *getWindow() const { return _window; }
    SDL_GLContext getContext() const { return _context; }

private:
    SDL_Window *_window;
    SDL_GLContext _context;

    int _width{1280};
    int _height{720};
};

} // namespace graphics