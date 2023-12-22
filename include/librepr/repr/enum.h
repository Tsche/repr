#pragma once
#include <type_traits>
#include <librepr/enum/reflect.h>
#include <librepr/macro/default.h>

#if USING(REPR_USE_MAGIC_ENUM)
#include <librepr/type/name.h>
#endif


namespace librepr {
template <typename T>
  requires std::is_enum_v<T>
std::string repr(T const& obj) {
  auto name = librepr::enum_name(obj);

  // TODO handle compound flags
  if (!name.empty()) {
#if USING(REPR_USE_MAGIC_ENUM)
    if constexpr (detail::is_scoped_enum<T>) {
      return librepr::get_name<T>() + "::" + std::string{name};
    }
#endif
    return std::string{name};
  }

  return librepr::repr(static_cast<std::underlying_type_t<T>>(obj));
}
}  // namespace librepr