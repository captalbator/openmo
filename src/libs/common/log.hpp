#pragma once

namespace common
{

// TODO: Logging configuration

class Log
{
public:
    enum Severity
    {
        LOG_SEVERITY_TRACE,
        LOG_SEVERITY_INFO,
        LOG_SEVERITY_WARN,
        LOG_SEVERITY_ERROR,
        LOG_SEVERITY_FATAL,
    };

    static void init();

    static void log(Severity severity, std::string const &fmt...);

private:
};

} // namespace common

#define LOG_TRACE(...) ::common::Log::log(::common::Log::LOG_SEVERITY_TRACE, __VA_ARGS__)
#define LOG_INFO(...) ::common::Log::log(::common::Log::LOG_SEVERITY_INFO, __VA_ARGS__)
#define LOG_WARN(...) ::common::Log::log(::common::Log::LOG_SEVERITY_WARN, __VA_ARGS__)
#define LOG_ERROR(...) ::common::Log::log(::common::Log::LOG_SEVERITY_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) ::common::Log::log(::common::Log::LOG_SEVERITY_FATAL, __VA_ARGS__)

#define LOG_ASSERT(condition, ...)                                                                 \
    if (!(condition))                                                                              \
    {                                                                                              \
        ::common::Log::log(::common::Log::LOG_SEVERITY_FATAL, __VA_ARGS__);                        \
        throw std::runtime_error("Assertion failed");                                              \
    }
