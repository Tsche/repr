#pragma once
#include <cstddef>
#include <concepts>
#include <string>
#include <type_traits>
#include "category.h"

#include <librepr/macro/format.h>

namespace librepr {

template <typename T>
struct Reflect;

template <typename T, std::size_t N>
  requires(not std::same_as<std::remove_reference_t<T>, char const>)  // TODO exclude other string literals
struct Reflect<T[N]> : category::Type<T[N]> {
  using type = T[N];
  using element_type = T;
  
  constexpr static auto extent = N;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor, T (&obj)[N]) {
    for (std::size_t idx = 0; idx < N; ++idx) {
      visitor(category::Value{obj[idx]});
    }
  }

  static std::string layout() { return REPR_FORMAT("{}[{}]", Reflect<T>::layout(), N); }
};

template <typename T>
struct Reflect<T[]> : category::Type<T[]> {  // NOLINT
  using type = T[];
  using element_type = T;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor, T const& /* obj */) { }

  static std::string layout() { return REPR_FORMAT("[{}]", Reflect<type>::layout()); }
};

}  // namespace librepr