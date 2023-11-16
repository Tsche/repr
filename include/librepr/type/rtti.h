#pragma once
#include <typeinfo>

#include <librepr/detail/default.h>
#include "demangle.h"

namespace librepr {

#if USING(REPR_RTTI)
inline
#endif
namespace rtti {

template <typename T>
std::string get_name_raw() {
#if USING(REPR_MSVC) && USING(REPR_WINDOWS)
  auto name = typeid(T).name();
  return librepr::detail::denoise_name(name);
#else
  return librepr::demangle(typeid(T).name());
#endif
}

}  // namespace rtti
}  // namespace librepr