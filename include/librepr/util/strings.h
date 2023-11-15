#pragma once

#include <string>

namespace librepr::detail {
inline void remove_all(std::string& text, const std::string& from) {
  for (auto at = text.find(from, 0); at != text.npos; at = text.find(from, at)) {
    text.erase(at, from.length());
  }
}
}  // namespace librepr::detail