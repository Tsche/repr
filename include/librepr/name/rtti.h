#pragma once
#include <typeinfo>

#include <librepr/macro/default.h>
#include <librepr/macro/platform.h>
#include "demangle.h"

namespace librepr::

#if USING(REPR_RTTI)
    inline
#endif
    rtti {

template <typename T>
std::string get_name_raw() {
#if USING(LIBREPR_PLATFORM_WINDOWS)
  auto name = typeid(T).name();
  return librepr::detail::undecorate_name(name);
#else
  return librepr::demangle(typeid(T).name());
#endif
}

}  // namespace librepr::rtti
