#pragma once
#include <string>
#include <format>


namespace librepr {
inline std::string repr(std::nullptr_t) {
  return "nullptr";
}

inline std::string repr(char const* obj) {
  return std::format("\"{}\"", obj);
}

inline std::string repr(void const* obj){
  return std::format("{}", obj);
}

}  // namespace librepr
