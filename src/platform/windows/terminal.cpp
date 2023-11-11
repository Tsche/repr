#include <librepr/terminal.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace librepr {

std::size_t get_terminal_width() {
  auto terminal_size = CONSOLE_SCREEN_BUFFER_INFO{};
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &terminal_size);
  return terminal_size.srWindow.Right - terminal_size.srWindow.Left + 1;
}

}