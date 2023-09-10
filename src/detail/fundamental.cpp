#include <format>
#include <detail/fundamental.h>

// Boolean type
std::string repr(bool obj) {
  return (obj) ? "true" : "false";
}

// Integer types
std::string repr(signed short obj) {
  return std::format("{}", obj);
}

std::string repr(unsigned short obj) {
  return std::format("{}", obj);
}

std::string repr(signed int obj) {
  return std::format("{}", obj);
}

std::string repr(unsigned int obj) {
  return std::format("{}U", obj);
}

std::string repr(signed long obj) {
  return std::format("{}L", obj);
}

std::string repr(unsigned long obj) {
  return std::format("{}UL", obj);
}

std::string repr(signed long long obj) {
  return std::format("{}LL", obj);
}

std::string repr(unsigned long long obj) {
  return std::format("{}ULL", obj);
}


// Floating-point types
std::string repr(float obj) {
  auto is_integer = static_cast<long long>(obj) == obj;
  return std::format("{}{}F", obj, (is_integer)?".0":"");
}

std::string repr(double obj) {
  auto is_integer = static_cast<long long>(obj) == obj;
  return std::format("{}{}", obj, (is_integer)?".0":"");
}
std::string repr(long double obj) {
  auto is_integer = static_cast<long long>(obj) == obj;
  return std::format("{}{}L", obj, (is_integer)?".0":"");
}

// Character types
std::string repr(char obj) {
  return std::format("'{:c}'", obj);
}