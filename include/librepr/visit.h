#pragma once
#include <type_traits>

#include <librepr/reflection/category.h>
#include <librepr/reflection/reflect.h>

namespace librepr {
template <typename T, typename V>
void visit(V&& visitor) {
  visitor(Reflect<T>{});
}

template <typename T, typename V>
void visit(V&& visitor, T&& obj) {
  using descend = Reflect<std::remove_reference_t<T>>;
  visitor(category::Value<descend>{obj});
}
}  // namespace librepr