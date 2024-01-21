#include "libs/common/log.hpp"
#include "libs/common/options.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/shader.hpp"
#include "libs/graphics/window.hpp"

const float triData[] = {
    0.0f, 0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -0.5f, -0.5f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f,
};

int main(int argc, char **argv)
{
    common::Log::init();

    graphics::WindowOptions options;
    options.title = "Gabbit - Level editor for Munch's Oddysee";
    options.w = 1280;
    options.h = 720;

    bool printHelp = false;
    OptionsParser opts("Gabbit - Level editor for Munch's Oddysee");
    opts.addArgument({"-w", "--width"}, &options.w, "Set the window width");
    opts.addArgument({"-h", "--height"}, &options.h, "Set the window height");
    opts.addArgument({"--help"}, &printHelp, "Print the help message");

    try
    {
        opts.parse(argc, argv);
    }
    catch (std::runtime_error const &e)
    {
        LOG_ERROR(e.what());
        return -1;
    }

    if (printHelp)
    {
        opts.printHelp();
        return 0;
    }

    graphics::Window window;
    window.init(options);

    graphics::Mesh tri;
    tri.setVertices(std::vector<float>(std::begin(triData), std::end(triData)));

    graphics::VertexLayout triLayout;
    triLayout.appendElement(3, graphics::VertexType::FLOAT);
    triLayout.appendElement(3, graphics::VertexType::FLOAT);

    tri.setLayout(triLayout);

    graphics::Shader vertexShader(graphics::ShaderType::VERTEX);
    graphics::Shader fragmentShader(graphics::ShaderType::FRAGMENT);

    vertexShader.setSource("assets/shaders/default.vert");
    vertexShader.compile();

    fragmentShader.setSource("assets/shaders/default.frag");
    fragmentShader.compile();

    graphics::ShaderProgram program;
    program.attach(&vertexShader);
    program.attach(&fragmentShader);

    program.link();

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                glViewport(0, 0, e.window.data1, e.window.data2);
                break;
            default:
                break;
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        tri.draw();

        glBindVertexArray(0);

        window.swap();
    }

    window.close();
    SDL_Quit();

    return 0;
}
