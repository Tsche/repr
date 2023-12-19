#pragma once
#include <typeinfo>

#include <librepr/detail/default.h>
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
  //TODO to be fixed, see feature/windows-support branch
  return name;
#else
  return librepr::demangle(typeid(T).name());
#endif
}

}  // namespace librepr::rtti
