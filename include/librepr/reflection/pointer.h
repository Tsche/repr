#pragma once
#include <string>
#include <format>
#include <type_traits>

#include <librepr/reflection/name.h>
#include <librepr/repr.h>

namespace librepr {
template <typename T>
struct Reflect;

template <typename T>
struct Reflect<T*> {
  using type = T;

  static std::string dump(T const* obj, bool /*with_type*/ = false, bool explicit_types = false) {
    if constexpr (!std::is_same_v<T, void> && !std::is_pointer_v<T>) {
      // try to reflect whatever the pointer is pointing to
      // disabled for void* and pointer-to-pointer T

      if (obj) {
        // don't attempt to dereference nullptr
        return std::format("new {}{{{}}}", librepr::get_name<T>(), Reflect<T>::dump(*obj, false, explicit_types));
      }
    }
    return std::format("({}){}", librepr::get_name<T*>(), librepr::repr(static_cast<void const*>(obj)));
  }

  static std::string layout() { return librepr::get_name<T*>(); }
};
}