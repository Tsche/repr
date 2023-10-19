#pragma once
#include <string>
#include <type_traits>

#include <librepr/literal.h>
#include <librepr/detail/concepts.h>
#include <librepr/reflection/name.h>


namespace librepr {
template <typename T>
struct Reflect;

template <detail::has_repr_member T>
struct Reflect<T> {
  using type = T;

  static std::string dump(T const& obj, bool /*with_type*/ = true, bool /*explicit_types*/ = false) {
    if (!is_literal_v<T>) {
      return librepr::get_name<T>() + obj.repr();
    }
    // TODO pass down explicit_types
    return obj.repr();
  }

  static std::string layout() {
    // TODO check for layout member function
    return librepr::get_name<T>();
  }
};


}