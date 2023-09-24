#pragma once
#include <type_traits>
#include <format>
#include <string>

#include <librepr/detail/visibility.h>
#include <librepr/type_info.h>
#include <librepr/object_info.h>

namespace librepr {

EXPORT ObjectInfo<std::nullptr_t, Literal> repr(std::nullptr_t obj);
EXPORT ObjectInfo<char const*, Literal> repr(char const* obj);

template <typename T>
  requires std::is_pointer<T>::value
EXPORT ObjectInfo<T, Literal> repr(T const& obj) {
  return {obj, std::format("({}){:p}", get_name<T>(), static_cast<void const*>(obj))};
}

}  // namespace librepr
