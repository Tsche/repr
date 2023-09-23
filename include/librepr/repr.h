#pragma once

#include <librepr/repr/fundamental.h>
#include <librepr/repr/pointer.h>
#include <librepr/repr/pair.h>
#include <librepr/repr/initializer_list.h>
#include <librepr/repr/repr_method.h>

namespace librepr {

template <typename T>
EXPORT std::string repr(T const& obj){
  // Fallback for when we couldn't find a suitable repr
  return std::format("[{} object at {:p}]", librepr::get_name<T>(), static_cast<void const*>(&obj));
}

template <typename T>
struct TypeName {
  static std::string to_string() noexcept { return librepr::get_name<T>(); }

  [[implicit]] operator std::string() const noexcept {  // NOLINT
    return to_string();
  }

  friend std::ostream& operator<<(std::ostream& ctx, TypeName<T> const& obj) { return ctx << obj.to_string(); }
};

}

// std::format support for librepr::TypeName<T>{} and nameof<T>
template <class T, class CharT>
struct std::formatter<librepr::TypeName<T>, CharT> : std::formatter<std::string, CharT> {  // NOLINT
  auto format(librepr::TypeName<T> const& obj, auto& ctx) const {
    return std::formatter<std::string, CharT>::format(obj.to_string(), ctx);
  }
};
