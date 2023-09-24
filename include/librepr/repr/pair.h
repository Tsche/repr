#pragma once
#include <string>
#include <format>

#include <librepr/detail/visibility.h>
#include <librepr/object_info.h>

namespace librepr {
template <typename T>
  requires requires(T obj) {
    typename T::first_type;
    typename T::second_type;
    requires std::is_same_v<decltype(obj.first), typename T::first_type> &&
                 std::is_same_v<decltype(obj.second), typename T::second_type>;
  }
EXPORT auto repr(T const& obj) {
  return ObjectInfo{obj, BracedList{librepr::repr(obj.first), librepr::repr(obj.second)}};
}
}