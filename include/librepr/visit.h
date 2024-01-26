#pragma once
#include <type_traits>

#include <librepr/reflection/category.h>
#include <librepr/reflection/reflect.h>
#include <librepr/visitors/cpp.h>
#include <librepr/visitors/python.h>

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

enum class Language { CPP, Python };

template <typename T, Language language = Language::CPP>
auto code_for() {
  if constexpr (language == Language::Python) {
    auto visitor = librepr::PythonVisitor{};
    librepr::visit<T>(visitor);
    return visitor.result.extract();
  }
  auto visitor = librepr::CppVisitor{};
  librepr::visit<T>(visitor);
  return visitor.result.extract();
}
}  // namespace librepr