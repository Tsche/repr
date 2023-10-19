#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <format>

#include <magic_enum.hpp>

#include <librepr/detail/concepts.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires std::is_enum_v<T>
struct Reflect<T> {
  using type = T;

  static std::string dump(T const& obj, bool /*with_type*/ = false, bool /*explicit_types*/ = false) {
    if constexpr (detail::is_scoped_enum<T>) {
      return std::format("{}::{}", librepr::get_name<T>(), librepr::repr(obj));
    } else {
      return std::format("{}", librepr::repr(obj));
    }
  }

  static std::string layout() {
    std::ostringstream list{};
    auto values = magic_enum::enum_names<T>();
    for (auto const& element : values) {
      if (&element != &*std::begin(values)) {
        list << " | ";
      }

      if constexpr (detail::is_scoped_enum<T>) {
        list << librepr::get_name<T>();
        list << "::";
      }

      list << element;
    }
    return list.str();
  }
};

}  // namespace librepr