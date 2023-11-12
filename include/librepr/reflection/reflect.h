#pragma once
#include <string>
#include <format>
#include <type_traits>


#include <librepr/repr.h>
#include <librepr/literal.h>

#include "name.h"

#include "aggregate.h"
#include "array.h"
#include "enum.h"
#include "init_list.h"
#include "pair.h"
#include "variant.h"

namespace librepr {

template <typename T>
struct Reflect {
  using type = T;

  static void visit(auto&& visitor, type const& obj) {
    visitor(obj);
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <>
struct Reflect<char const*> {
  static void visit(auto&& visitor, char const* obj) {
    visitor(obj);
  }

  static std::string layout() { return "str"; }
};

}  // namespace librepr