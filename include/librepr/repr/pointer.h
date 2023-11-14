#pragma once
#include <string>
#include <format>

#include <librepr/detail/visibility.h>

namespace librepr {
EXPORT inline std::string repr(std::nullptr_t) {
  return "nullptr";
}

EXPORT inline std::string repr(char const* obj) {
  return std::format("\"{}\"", obj);
}

EXPORT inline std::string repr(void const* obj){
  return std::format("{}", obj);
}

}  // namespace librepr
