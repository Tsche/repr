#pragma once
#include <string>
#include <format>
#include <type_traits>


#include <librepr/repr.h>
#include <librepr/literal.h>

#include "name.h"

#include "aggregate.h"
#include "array.h"
#include "custom.h"
#include "enum.h"
#include "init_list.h"
#include "pair.h"
#include "pointer.h"
#include "variant.h"

namespace librepr {

template <typename T>
struct Reflect {
  using type = T;

  static std::string dump(T const& obj, bool with_type = true, bool /*explicit_types*/ = false) {
    if (with_type && !is_literal_v<T>) {
      return librepr::get_name<T>() + librepr::repr(obj);
    }
    return librepr::repr(obj);
  }

  static std::string layout() { return librepr::get_name<T>(); }
};

template <>
struct Reflect<char const*> {
  static std::string dump(char const* obj, bool /*with_type*/ = false, bool /*explicit_types*/ = false) {
    return librepr::repr(obj);
  }

  static std::string layout() { return "str"; }
};

}  // namespace librepr