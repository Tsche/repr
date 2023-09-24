#include <format>
#include <librepr/repr/fundamental.h>

namespace librepr {
// Boolean type
auto repr(bool const& obj) -> ObjectInfo<bool, Literal> {
  return {obj, (obj) ? "true" : "false"};
}

// Integer types
auto repr(signed short const& obj) -> ObjectInfo<signed short, Literal> {
  return {obj, std::format("{}", obj)};
}

auto repr(unsigned short const& obj) -> ObjectInfo<unsigned short, Literal> {
  return {obj, std::format("{}", obj)};
}
auto repr(signed int const& obj) -> ObjectInfo<signed int, Literal> {
  return {obj, std::format("{}", obj)};
}
auto repr(unsigned int const& obj) -> ObjectInfo<unsigned int, Literal> {
  return {obj, std::format("{}U", obj)};
}
auto repr(signed long const& obj) -> ObjectInfo<signed long, Literal> {
  return {obj, std::format("{}L", obj)};
}
auto repr(unsigned long const& obj) -> ObjectInfo<unsigned long, Literal> {
  return {obj, std::format("{}UL", obj)};
}
auto repr(signed long long const& obj) -> ObjectInfo<signed long long, Literal> {
  return {obj, std::format("{}LL", obj)};
}
auto repr(unsigned long long const& obj) -> ObjectInfo<unsigned long long, Literal> {
  return {obj, std::format("{}ULL", obj)};
}

// Floating-point types
auto repr(float const& obj) -> ObjectInfo<float, Literal> {
  auto is_integer = static_cast<long long>(obj) == obj;
  return {obj, std::format("{}{}F", obj, (is_integer) ? ".0" : "")};
}

auto repr(double const& obj) -> ObjectInfo<double, Literal> {
  auto is_integer = static_cast<long long>(obj) == obj;
  return {obj, std::format("{}{}", obj, (is_integer) ? ".0" : "")};
}

auto repr(long double const& obj) -> ObjectInfo<long double, Literal> {
  auto is_integer = static_cast<long long>(obj) == obj;
  return {obj, std::format("{}{}L", obj, (is_integer) ? ".0" : "")};
}

// Character types
auto repr(char const& obj) -> ObjectInfo<char, Literal> {
  return {obj, std::format("'{:c}'", obj)};
}
}  // namespace librepr