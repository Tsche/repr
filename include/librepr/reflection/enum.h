#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <format>

#include <magic_enum.hpp>
#include <librepr/reflection/name.h>

#include <librepr/detail/concepts.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires std::is_enum_v<T>
struct Reflect<T> {
  using type = T;

  static void visit(auto&& visitor, type const& obj) {
    //TODO check typename against detail::is_scoped_enum<T>
    // return std::format("{}::{}", librepr::get_name<T>(), librepr::repr(obj));
    visitor(obj);
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