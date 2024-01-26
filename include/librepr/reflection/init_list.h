#pragma once
#include <string>
#include <initializer_list>
#include <ranges>
#include <type_traits>

#include "category.h"

namespace librepr {
template <typename T>
struct Reflect;

template <std::ranges::range T>
  requires std::constructible_from<std::initializer_list<typename T::value_type>>
struct Reflect<T> : category::Type<T>{
  using type = T;
  using element_type = typename T::value_type;
  constexpr static bool can_descend = true;

  template <typename V> 
  static void visit(V&& visitor, T& obj){
    for (auto&& value : obj){
      visitor(category::Value{value});
    }
  }
};
}  // namespace librepr