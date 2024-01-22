#include "libs/common/log.hpp"
#include "libs/common/options.hpp"
#include "libs/graphics/mesh.hpp"
#include "libs/graphics/shader.hpp"
#include "libs/graphics/window.hpp"

#include "editor.hpp"

int main(int argc, char **argv)
{
    common::Log::init();

    EditorOptions opts;
    opts.windowOpts.title = "Gabbit - Level editor for Munch's Oddysee";
    opts.windowOpts.w = 1280;
    opts.windowOpts.h = 720;

    std::string filePath;

    bool printHelp = false;
    OptionsParser args("Gabbit - Level editor for Munch's Oddysee");
    args.addArgument({"-w", "--width"}, &opts.windowOpts.w, "Set the window width");
    args.addArgument({"-h", "--height"}, &opts.windowOpts.h, "Set the window height");
    args.addArgument({"-f", "--file"}, &filePath, "Set a region file to open");
    args.addArgument({"--help"}, &printHelp, "Print the help message");

    try
    {
        args.parse(argc, argv);
    }
    catch (std::runtime_error const &e)
    {
        LOG_ERROR(e.what());
        return -1;
    }

    if (printHelp)
    {
        args.printHelp();
        return 0;
    }

    if (!filePath.empty())
    {
        opts.openFile = filePath;
    }

    Editor e(opts);
    try
    {
        e.run();
    }
    catch (std::exception const &e)
    {
        LOG_ERROR(e.what());
        return -1;
    }

    return 0;
}
