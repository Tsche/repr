#pragma once
#include <string>

#include "category.h"
#include "aggregate.h"
#include "array.h"
#include "enum.h"
#include "init_list.h"
#include "pair.h"
#include "variant.h"
#include "custom.h"

namespace librepr {

template <typename T>
struct Reflect : category::Type<T>{
  using type = T;

  template <typename V>
  static void visit(V&& visitor, T& obj) {
    visitor(category::Value{obj});
  }
};

template <>
struct Reflect<char const*> : category::Type<char const*>{
  using type = char const*;

  template <typename V>
  static void visit(V&& visitor, char const* obj) {
    visitor(category::Value<Reflect<char const*>>{obj});
  }
};

template <typename T>
// TODO check Settings -> ADL -> member function -> builtin
using reflect = Reflect<T>;


template <typename T>
Reflect(T&) -> Reflect<T>;

}  // namespace librepr