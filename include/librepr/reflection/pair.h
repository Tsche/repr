#pragma once
#include <string>
#include <type_traits>

#include <librepr/util/concepts.h>
#include "category.h"
namespace librepr {
template <typename T>
struct Reflect;

template <detail::pair_like T>
struct Reflect<T> {
  using type        = T;
  using first_type  = std::remove_cv_t<std::remove_reference_t<typename T::first_type>>;
  using second_type = std::remove_cv_t<std::remove_reference_t<typename T::second_type>>;
  constexpr static bool can_descend = true;

  template <typename V>
  static void visit(V&& visitor, T& obj) {
    visitor(category::Value{obj.first});
    visitor(category::Value{obj.second});
  }
};

}  // namespace librepr