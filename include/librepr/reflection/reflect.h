#pragma once
#include <string>

#include <librepr/visitors/visitor.h>
#include <librepr/type/name.h>

#include "aggregate.h"
#include "array.h"
#include "enum.h"
#include "init_list.h"
#include "pair.h"
#include "variant.h"
#include "custom.h"

namespace librepr {

template <typename T>
struct Reflect {
  using type = T;

  template <typename V>
  static void visit(V&& visitor, type const& obj) {
    Visit::value(visitor, obj);
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <>
struct Reflect<char const*> {
  template <typename V>
  static void visit(V&& visitor, char const* obj) {
    Visit::value(visitor, obj);
  }

  static std::string layout() { return "str"; }
};

template <typename T>
// TODO check Settings -> ADL -> member function -> builtin
using reflect = Reflect<T>;

}  // namespace librepr