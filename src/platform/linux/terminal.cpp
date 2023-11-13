#include <librepr/terminal.h>

#include <sys/ioctl.h>
#include <unistd.h>

namespace librepr {

std::size_t get_terminal_width() {
  auto terminal_size = ::winsize{};
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
  return terminal_size.ws_col;
}

}