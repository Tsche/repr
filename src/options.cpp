#include <librepr/options.h>

namespace librepr {
bool Options::should_print_type(std::size_t level) const {
  if (level == 0) {
    return print_type;
  }
  return explicit_types;
}

std::string Options::with_indent(std::string const& what, std::size_t level) const {
  if (indent != 0){
    return '\n' + std::string(indent * level, ' ') + what;
  }
  return what;
}
}  // namespace librepr