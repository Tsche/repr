#pragma once
#include <string>
#include <typeinfo>

#include <librepr/feature.h> 
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
#if USING(LIBREPR_COMPILER_MSVC)
  auto name = typeid(T).name();
  return librepr::detail::undecorate_name(name);
#else
  return librepr::demangle(typeid(T).name());
#endif
}

}  // namespace librepr::rtti
