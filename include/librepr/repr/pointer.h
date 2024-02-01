#pragma once
#include <string>
#include <librepr/macro/format.h>


namespace librepr {
inline std::string repr(std::nullptr_t) {
  return "nullptr";
}

inline std::string repr(char const* obj) {
  return REPR_FORMAT("\"{}\"", obj);
}

inline std::string repr(void const* obj){
  return REPR_FORMAT("{}", obj);
}

}  // namespace librepr
