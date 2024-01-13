#pragma once
#include <string>
#include <sstream>
#include <type_traits>
#include <concepts>

#include <librepr/macro/format.h>
#include <librepr/name/type.h>
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
    Visit::type<T[N]>(visitor);
    ScopeGuard guard{visitor};
    for (std::size_t idx = 0; idx < N; ++idx) {
      Reflect<type>::visit(visitor, obj[idx]);
    }
  }

  static std::string layout() { return REPR_FORMAT("{}[{}]", Reflect<type>::layout(), N); }
};

template <typename T>
struct Reflect<T[]> {  // NOLINT
  using type = T;

  template <typename V>
  static void visit(V&& visitor, T const& /* obj */) {
    Visit::type<T[]>(visitor);
    ScopeGuard guard{visitor};
  }

  static std::string layout() { return REPR_FORMAT("[{}]", Reflect<type>::layout()); }
};

}  // namespace librepr