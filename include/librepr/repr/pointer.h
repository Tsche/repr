#pragma once

#include <format>
#include <string>
#include <librepr/detail/visibility.h>
#include <librepr/typename.h>

namespace librepr {

EXPORT std::string repr(std::nullptr_t obj);
EXPORT std::string repr(char const* obj);

template <typename T>
  requires std::is_pointer<T>::value
EXPORT std::string repr(T const& obj) {
  return std::format("({}){:p}", get_name<T>(), static_cast<void const*>(obj));  
}

}  // namespace librepr
