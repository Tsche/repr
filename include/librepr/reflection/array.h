#pragma once
#include <format>
#include <string>
#include <sstream>
#include <type_traits>

#include <librepr/reflection/name.h>
#include "visitor.h"

namespace librepr {
template <typename T>
struct Reflect;

template <typename T, std::size_t N>
struct Reflect<T[N]> {
  using type = T;

  static void visit(auto&& visitor, T const (&obj)[N]) {
    ScopeGuard guard{visitor, std::type_identity<T[N]>{}};
    for (std::size_t idx = 0; idx < N; ++idx) {
      Reflect<type>::visit(std::forward<decltype(visitor)>(visitor), obj[idx]);
    }
  }

  static std::string layout() { return std::format("{}[{}]", Reflect<type>::layout(), N); }
};

template <typename T>
struct Reflect<T[]> {  // NOLINT
  using type = T;

  static void visit(auto&& visitor, T const& obj) {
    ScopeGuard guard{visitor, std::type_identity<T[]>{}};
  }

  static std::string layout() { return std::format("[{}]", Reflect<type>::layout()); }
};

}  // namespace librepr