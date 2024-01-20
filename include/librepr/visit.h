#pragma once

#include <librepr/reflection/reflect.h>
#include <type_traits>

namespace librepr {
  template <typename T, typename V>
  void visit(V&& visitor){
    visitor(Reflect<T>{});
  }

  template <typename T, typename V>
  void visit(V&& visitor, T&& obj){
    using descend = Reflect<std::remove_reference_t<T>>;
    visitor(category::Value<descend>{obj});
  }
}