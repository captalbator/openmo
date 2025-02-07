#pragma once

#include "fmt/chrono.h"
#include "fmt/color.h"

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

  template <typename... Args>
  static void log(Severity severity, std::string_view fmt, Args &&...args)
  {
    std::time_t t = std::time(nullptr);

    auto msg = fmt::format(fmt, std::forward<Args>(args)...);

    switch (severity)
    {
    case LOG_SEVERITY_TRACE:
      fmt::print(fmt::fg(fmt::color::antique_white), "[{:%H:%M:%S}] TRACE: {}\n", fmt::localtime(t),
                 msg);
      break;
    case LOG_SEVERITY_INFO:
      fmt::print(fmt::fg(fmt::color::white_smoke), "[{:%H:%M:%S}] INFO: {}\n", fmt::localtime(t),
                 msg);
      break;
    case LOG_SEVERITY_WARN:
      fmt::print(fmt::fg(fmt::color::golden_rod), "[{:%H:%M:%S}] WARN: {}\n", fmt::localtime(t),
                 msg);
      break;
    case LOG_SEVERITY_ERROR:
      fmt::print(fmt::fg(fmt::color::crimson), "[{:%H:%M:%S}] ERROR: {}\n", fmt::localtime(t), msg);
      break;
    case LOG_SEVERITY_FATAL:
      fmt::print(fmt::fg(fmt::color::crimson), "[{:%H:%M:%S}] FATAL: {}\n", fmt::localtime(t), msg);
      break;
    default:
      break;
    }
  }

private:
};

} // namespace common

#define LOG_CALL(level, ...) ::common::Log::log(level, __VA_ARGS__)

#define LOG_TRACE(...) LOG_CALL(::common::Log::LOG_SEVERITY_INFO, __VA_ARGS__)
#define LOG_INFO(...) LOG_CALL(::common::Log::LOG_SEVERITY_INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG_CALL(::common::Log::LOG_SEVERITY_WARN, __VA_ARGS__)
#define LOG_ERROR(...) LOG_CALL(::common::Log::LOG_SEVERITY_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) LOG_CALL(::common::Log::LOG_SEVERITY_FATAL, __VA_ARGS__)

#define LOG_ASSERT(condition, ...)                                                                 \
  if (!(condition))                                                                                \
  {                                                                                                \
    LOG_CALL(::common::Log::LOG_SEVERITY_FATAL, __VA_ARGS__);                                      \
    throw std::runtime_error("Assertion failed");                                                  \
  }
