#pragma once

#include <librepr/reflection/reflect.h>
#include <type_traits>

#include <librepr/visitors/cpp.h>

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

template <typename T>
auto code_for() {
  auto visitor = librepr::CppVisitor{};
  librepr::visit<T>(visitor);
  return visitor.result.extract();
}
}  // namespace librepr