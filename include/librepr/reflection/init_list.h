#pragma once
#include <string>
#include <initializer_list>
#include <ranges>
#include <type_traits>

#include <librepr/macro/format.h>
#include <librepr/name/type.h>
#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;

template <std::ranges::range T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
struct Reflect<T> {
  using type = typename T::value_type;

  template <typename V>
  static void visit(V&& visitor, T const& obj) {
    Visit::type<T>(visitor);
    ScopeGuard guard{visitor};

    for (auto const& element : obj) {
      Reflect<type>::visit(std::forward<V>(visitor), element);
    }
  }

  static std::string layout() { return REPR_FORMAT("[{}]", Reflect<type>::layout()); }
};
}  // namespace librepr