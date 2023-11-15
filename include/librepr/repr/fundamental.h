#pragma once
#include <string>
#include <format>
#include <cmath>

namespace librepr {

// Boolean type
inline std::string repr(bool obj) {
  return obj ? "true" : "false";
}

// Integer types
inline std::string repr(signed short obj) {
  return std::format("{}", obj);
}

inline std::string repr(unsigned short obj) {
  return std::format("{}", obj);
}
inline std::string repr(signed int obj) {
  return std::format("{}", obj);
}
inline std::string repr(unsigned int obj) {
  return std::format("{}U", obj);
}
inline std::string repr(signed long obj) {
  return std::format("{}L", obj);
}
inline std::string repr(unsigned long obj) {
  return std::format("{}UL", obj);
}
inline std::string repr(signed long long obj) {
  return std::format("{}LL", obj);
}
inline std::string repr(unsigned long long obj) {
  return std::format("{}ULL", obj);
}

// Floating-point types
inline std::string repr(float obj) {
  auto is_integer = rintf(obj) == obj;
  return std::format("{}{}F", obj, is_integer ? ".0" : "");
}

inline std::string repr(double obj) {
  auto is_integer = rint(obj) == obj;
  return std::format("{}{}", obj, is_integer ? ".0" : "");
}

inline std::string repr(long double obj) {
  auto is_integer = rintl(obj) == obj;
  return std::format("{}{}L", obj, is_integer ? ".0" : "");
}

// Character types
inline std::string repr(char obj) {
  return std::format("'{:c}'", obj);
}

}  // namespace librepr