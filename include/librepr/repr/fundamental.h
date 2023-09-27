#pragma once
#include <string>
#include <librepr/detail/visibility.h>
namespace librepr {

// Boolean type
EXPORT std::string repr(bool const& obj);

// Integer types
EXPORT std::string repr(signed short const& obj);
EXPORT std::string repr(unsigned short const& obj);
EXPORT std::string repr(signed int const& obj);
EXPORT std::string repr(unsigned int const& obj);
EXPORT std::string repr(signed long const& obj);
EXPORT std::string repr(unsigned long const& obj);
EXPORT std::string repr(signed long long const& obj);
EXPORT std::string repr(unsigned long long const& obj);

// Floating-point types
EXPORT std::string repr(float const& obj);
EXPORT std::string repr(double const& obj);
EXPORT std::string repr(long double const& obj);

// Character types
EXPORT std::string repr(char const& obj);

}  // namespace librepr