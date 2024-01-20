#pragma once
#include <string>

#include <librepr/visitors/visitor.h>
#include <librepr/name/type.h>

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

  /*template <typename V>
  static void visit(V&& visitor, type const& obj) {
    Visit::value(visitor, obj);
  }*/

  template <typename V>
  static void visit(V&& visitor, T& obj) {
    visitor(category::Value{obj});
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <>
struct Reflect<char const*> : category::Type<char const*>{
  using type = char const*;

  /*template <typename V>
  static void visit(V&& visitor, char const* obj) {
    Visit::value(visitor, obj);
  }*/

  template <typename V>
  static void visit(V&& visitor, char const* obj) {
    visitor(category::Value<Reflect<char const*>>{obj});
  }

  static auto type_name() { return librepr::get_name<type>(); }
  static std::string layout() { return "str"; }
};

template <typename T>
// TODO check Settings -> ADL -> member function -> builtin
using reflect = Reflect<T>;


template <typename T>
Reflect(T&) -> Reflect<T>;

}  // namespace librepr