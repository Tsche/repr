#include <librepr/options.h>

namespace librepr {
bool Options::should_print_type(std::size_t level) const {
  if (level == 0) {
    return print_type;
  }
  return explicit_types;
}

}  // namespace librepr