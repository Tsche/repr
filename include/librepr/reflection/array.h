#pragma once
#include <format>
#include <string>
#include <sstream>
#include <type_traits>
#include <concepts>

#include <librepr/type/name.h>
#include <librepr/visitors/visitor.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T, std::size_t N>
requires (not std::same_as<T, char>) //TODO exclude other string literals
struct Reflect<T[N]> {
  using type = T;

  template <typename V>
  static void visit(V&& visitor, T const (&obj)[N]) {
    ScopeGuard guard{visitor, std::type_identity<T[N]>{}};
    for (std::size_t idx = 0; idx < N; ++idx) {
      Reflect<type>::visit(std::forward<V>(visitor), obj[idx]);
    }
  }

  static std::string layout() { return std::format("{}[{}]", Reflect<type>::layout(), N); }
};

template <typename T>
struct Reflect<T[]> {  // NOLINT
  using type = T;

  template <typename V>
  static void visit(V&& visitor, T const& /* obj */) {
    ScopeGuard guard{std::forward<V>(visitor), std::type_identity<T[]>{}};
  }

  static std::string layout() { return std::format("[{}]", Reflect<type>::layout()); }
};

}  // namespace librepr