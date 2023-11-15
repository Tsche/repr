#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <format>

#include <magic_enum.hpp>
#include <librepr/type/name.h>
#include <librepr/visitors/visitor.h>
#include <librepr/util/concepts.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires std::is_enum_v<T>
struct Reflect<T> {
  using type = T;

  template <Visitor::Values V>
  static void visit(V&& visitor, type const& obj) {
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