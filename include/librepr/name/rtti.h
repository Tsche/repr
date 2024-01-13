#pragma once
#include <typeinfo>

#include <librepr/macro/default.h>
#include "demangle.h"

namespace librepr::

#if USING(REPR_RTTI)
    inline
#endif
    rtti {

template <typename T>
std::string get_name_raw() {
#if defined(_WIN32)
  auto name = typeid(T).name();
  return librepr::detail::undecorate_name(name);
#else
  return librepr::demangle(typeid(T).name());
#endif
}

}  // namespace librepr::rtti
