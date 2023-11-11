#pragma once
#include <string>
#include <type_traits>

#include <librepr/literal.h>
#include <librepr/detail/concepts.h>
#include <librepr/reflection/name.h>


namespace librepr {
template <typename T>
struct Reflect;

//TODO figure out if this is needed
template <detail::has_repr_member T>
struct Reflect<T> {
  using type = T;

  static void visit(auto&& visitor, type const& obj) {
    visitor(obj);
  }

  static std::string layout() {
    // TODO check for layout member function
    return librepr::get_name<T>();
  }
};


}