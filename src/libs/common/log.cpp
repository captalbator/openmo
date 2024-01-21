#include "log.hpp"

#include "fmt/chrono.h"
#include "fmt/color.h"

namespace common
{

void Log::init()
{
    // Enable terminal colors on windows
#ifdef __WIN32__
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void Log::log(Severity severity, std::string const &fmt...)
{
    std::time_t t = std::time(nullptr);

    switch (severity)
    {
    case LOG_SEVERITY_TRACE:
        fmt::print(fmt::fg(fmt::color::antique_white), "[{:%H:%M:%S}] TRACE: {}\n",
                   fmt::localtime(t), fmt);
        break;
    case LOG_SEVERITY_INFO:
        fmt::print(fmt::fg(fmt::color::white_smoke), "[{:%H:%M:%S}] INFO: {}\n", fmt::localtime(t),
                   fmt);
        break;
    case LOG_SEVERITY_WARN:
        fmt::print(fmt::fg(fmt::color::golden_rod), "[{:%H:%M:%S}] WARN: {}\n", fmt::localtime(t),
                   fmt);
        break;
    case LOG_SEVERITY_ERROR:
        fmt::print(fmt::fg(fmt::color::red), "[{:%H:%M:%S}] ERROR: {}\n", fmt::localtime(t), fmt);
        break;
    case LOG_SEVERITY_FATAL:
        fmt::print(fmt::fg(fmt::color::red), "[{:%H:%M:%S}] FATAL: {}\n", fmt::localtime(t), fmt);
        break;
    default:
        break;
    }
}

} // namespace common
