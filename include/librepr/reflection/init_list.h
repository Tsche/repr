#pragma once
#include <format>
#include <string>
#include <initializer_list>
#include <ranges>
#include <type_traits>

#include <librepr/type/name.h>
#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;

template <std::ranges::range T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
struct Reflect<T> {
  using type = typename T::value_type;

  static void visit(Visitor::Values auto&& visitor, T const& obj) {
    ScopeGuard guard{visitor, std::type_identity<T>{}};
    for (auto const& element : obj) {
      Reflect<type>::visit(std::forward<decltype(visitor)>(visitor), element);
    }
  }

  static std::string layout() { return std::format("[{}]", Reflect<type>::layout()); }
};
}  // namespace librepr