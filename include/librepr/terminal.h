#pragma once
#include <cstdint>

#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <Windows.h>
#else
  #include <sys/ioctl.h>
  #include <unistd.h>
#endif

namespace librepr {
[[nodiscard]] inline std::size_t get_terminal_width() {
#if defined(_WIN32)
  auto terminal_size = CONSOLE_SCREEN_BUFFER_INFO{};
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &terminal_size);
  return unsigned(terminal_size.srWindow.Right - terminal_size.srWindow.Left) + 1U;
#else
  auto terminal_size = ::winsize{};
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
  return terminal_size.ws_col;
#endif
}
}  // namespace librepr