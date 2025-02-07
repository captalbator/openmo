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

} // namespace common
