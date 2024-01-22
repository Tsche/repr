#pragma once
#include <cstddef>

#include <librepr/terminal.h>

namespace librepr {
struct Options {
  enum Source { RTTI, CTTI, SOURCE_LOCATION };

  bool print_type       = true;
  bool explicit_types   = false;
  Source name_source    = RTTI;
  std::size_t indent    = 0;
  std::size_t max_width = librepr::get_terminal_width();

  [[nodiscard]] bool should_print_type(std::size_t level) const {
    if (level == 0) {
      return print_type;
    }
    return explicit_types;
  }
};
}  // namespace librepr