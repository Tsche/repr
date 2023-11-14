#pragma once
#include <string>
#include <format>
#include <cmath>

#include <librepr/detail/visibility.h>
namespace librepr {

// Boolean type
EXPORT inline std::string repr(bool obj) {
  return obj ? "true" : "false";
}

// Integer types
EXPORT inline std::string repr(signed short obj) {
  return std::format("{}", obj);
}

EXPORT inline std::string repr(unsigned short obj) {
  return std::format("{}", obj);
}
EXPORT inline std::string repr(signed int obj) {
  return std::format("{}", obj);
}
EXPORT inline std::string repr(unsigned int obj) {
  return std::format("{}U", obj);
}
EXPORT inline std::string repr(signed long obj) {
  return std::format("{}L", obj);
}
EXPORT inline std::string repr(unsigned long obj) {
  return std::format("{}UL", obj);
}
EXPORT inline std::string repr(signed long long obj) {
  return std::format("{}LL", obj);
}
EXPORT inline std::string repr(unsigned long long obj) {
  return std::format("{}ULL", obj);
}

// Floating-point types
EXPORT inline std::string repr(float obj) {
  auto is_integer = rintf(obj) == obj;
  return std::format("{}{}F", obj, is_integer ? ".0" : "");
}

EXPORT inline std::string repr(double obj) {
  auto is_integer = rint(obj) == obj;
  return std::format("{}{}", obj, is_integer ? ".0" : "");
}

EXPORT inline std::string repr(long double obj) {
  auto is_integer = rintl(obj) == obj;
  return std::format("{}{}L", obj, is_integer ? ".0" : "");
}

// Character types
EXPORT inline std::string repr(char obj) {
  return std::format("'{:c}'", obj);
}

}  // namespace librepr