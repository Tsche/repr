#pragma once
#include <typeinfo>

#include <librepr/detail/default.h>
#include <librepr/demangle.h>

namespace librepr {

#if USING(REPR_RTTI)
inline
#endif
namespace rtti {

template <typename T>
std::string get_name_raw() {
#if defined(_WIN32)
  auto name = typeid(T).name();
  return librepr::detail::undecorate_name(name);
#else
  return librepr::demangle(typeid(T).name());
#endif
}

}  // namespace rtti
}  // namespace librepr