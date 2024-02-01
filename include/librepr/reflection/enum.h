#pragma once
#include <type_traits>

#include <librepr/feature.h>
#include <librepr/macro/default.h>
#include <librepr/util/concepts.h>
#include <librepr/enum/reflect.h>

#include "category.h"

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
  requires std::is_enum_v<T>
struct Reflect<T> : category::Type<T> {
  using type                             = T;
  constexpr static auto enumerator_names = librepr::enum_names<T>();
  constexpr static bool can_descend      = false;
  // TODO enum values -> names mapping

  template <typename V>
  static void visit(V&& visitor, T& obj) {
    visitor(category::Value{obj});
  }
};

}  // namespace librepr