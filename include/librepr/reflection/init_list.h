#pragma once
#include <format>
#include <string>
#include <initializer_list>
#include <ranges>
#include <sstream>
#include <concepts>

#include <librepr/reflection/name.h>

namespace librepr {
template <typename T>
struct Reflect;

template <std::ranges::range T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
struct Reflect<T> {
  using type = typename T::value_type;

  static std::string dump(T const& obj, bool with_type = true, bool explicit_types = false) {
    std::ostringstream list{};

    if (with_type) {
      list << librepr::get_name<T>();
    }

    list << '{';
    for (auto const& element : obj) {
      if (&element != &*std::begin(obj)) {
        list << ", ";
      }

      list << Reflect<type>::dump(element, explicit_types, explicit_types);
    }
    list << '}';
    return list.str();
  }

  static std::string layout() { return std::format("[{}]", Reflect<type>::layout()); }
};
}  // namespace librepr