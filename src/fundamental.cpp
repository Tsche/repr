#include <format>
#include <cmath>

#include <librepr/repr/fundamental.h>

namespace librepr {
// Boolean type
std::string repr(bool const& obj) {
  return obj ? "true" : "false";
}

// Integer types
std::string repr(signed short const& obj) {
  return std::format("{}", obj);
}

std::string repr(unsigned short const& obj) {
  return std::format("{}", obj);
}
std::string repr(signed int const& obj) {
  return std::format("{}", obj);
}
std::string repr(unsigned int const& obj) {
  return std::format("{}U", obj);
}
std::string repr(signed long const& obj) {
  return std::format("{}L", obj);
}
std::string repr(unsigned long const& obj) {
  return std::format("{}UL", obj);
}
std::string repr(signed long long const& obj) {
  return std::format("{}LL", obj);
}
std::string repr(unsigned long long const& obj) {
  return std::format("{}ULL", obj);
}

// Floating-point types
std::string repr(float const& obj) {
  auto is_integer = rintf(obj) == obj;
  return std::format("{}{}F", obj, is_integer ? ".0" : "");
}

std::string repr(double const& obj) {
  auto is_integer = rint(obj) == obj;
  return std::format("{}{}", obj, is_integer ? ".0" : "");
}

std::string repr(long double const& obj) {
  auto is_integer = rintl(obj) == obj;
  return std::format("{}{}L", obj, is_integer ? ".0" : "");
}

// Character types
std::string repr(char const& obj) {
  return std::format("'{:c}'", obj);
}
}  // namespace librepr