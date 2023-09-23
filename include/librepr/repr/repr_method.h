#pragma once
#include <librepr/detail/visibility.h>
#include <string>
#include <format>
#include <concepts>

namespace librepr {

template <typename T>
  requires requires(T const obj) {
    { obj.repr() } -> std::same_as<std::string>;
  }
EXPORT std::string repr(T const& obj) {
  return obj.repr();
}

}  // namespace librepr