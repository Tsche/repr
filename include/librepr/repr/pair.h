#pragma once
#include <librepr/detail/visibility.h>
#include <string>
#include <format>

namespace librepr {
template <typename T>
  requires requires(T obj) {
    typename T::first_type;
    typename T::second_type;
    requires std::is_same_v<decltype(obj.first), typename T::first_type> &&
                 std::is_same_v<decltype(obj.second), typename T::second_type>;
  }
EXPORT std::string repr(T const& obj) {
  return librepr::get_name<T>() + std::format("{{{}, {}}}", librepr::repr(obj.first), librepr::repr(obj.second));
}
}