#pragma once
#include <string>
#include <cmath>
#include <librepr/detail/format.h>

namespace librepr {

// Boolean type
inline std::string repr(bool obj) {
  return obj ? "true" : "false";
}

// Integer types
inline std::string repr(signed short obj) {
  return REPR_FORMAT("{}", obj);
}

inline std::string repr(unsigned short obj) {
  return REPR_FORMAT("{}", obj);
}
inline std::string repr(signed int obj) {
  return REPR_FORMAT("{}", obj);
}
inline std::string repr(unsigned int obj) {
  return REPR_FORMAT("{}U", obj);
}
inline std::string repr(signed long obj) {
  return REPR_FORMAT("{}L", obj);
}
inline std::string repr(unsigned long obj) {
  return REPR_FORMAT("{}UL", obj);
}
inline std::string repr(signed long long obj) {
  return REPR_FORMAT("{}LL", obj);
}
inline std::string repr(unsigned long long obj) {
  return REPR_FORMAT("{}ULL", obj);
}

// Floating-point types
inline std::string repr(float obj) {
  auto is_integer = rintf(obj) == obj;
  return REPR_FORMAT("{}{}F", obj, is_integer ? ".0" : "");
}

inline std::string repr(double obj) {
  auto is_integer = rint(obj) == obj;
  return REPR_FORMAT("{}{}", obj, is_integer ? ".0" : "");
}

inline std::string repr(long double obj) {
  auto is_integer = rintl(obj) == obj;
  return REPR_FORMAT("{}{}L", obj, is_integer ? ".0" : "");
}

// Character types
inline std::string repr(char obj) {
  return REPR_FORMAT("'{:c}'", obj);
}

}  // namespace librepr