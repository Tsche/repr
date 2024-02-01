#pragma once
#include <string>
#include <type_traits>

#include <librepr/util/concepts.h>
#include "category.h"
#include "librepr/util/collections/list.h"
namespace librepr {
template <typename T>
struct Reflect;

template <detail::pair_like T>
struct Reflect<T> : category::Type<T> {
  using type                        = T;
  using first_type                  = T::first_type;
  using second_type                 = T::second_type;
  using members                     = TypeList<first_type, second_type>::template map<librepr::Reflect>;
  constexpr static bool can_descend = true;
  constexpr static bool iterable    = false;
  constexpr static bool tuple_like  = true;

  template <typename V>
  static void visit(V&& visitor, T& obj) {
    visitor(category::Value{obj.first});
    visitor(category::Value{obj.second});
  }

  template <typename V>
  static void visit(V&& visitor) {
    visitor(Reflect<first_type>{});
    visitor(Reflect<second_type>{});
  }
};

}  // namespace librepr