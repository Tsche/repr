#pragma once
#include <string>
#include <librepr/detail/visibility.h>
#include <librepr/object_info.h>

namespace librepr {

// Boolean type
EXPORT auto repr(bool const& obj) -> ObjectInfo<bool, Literal>;

// Integer types
EXPORT auto repr(signed short const& obj) -> ObjectInfo<signed short, Literal>;
EXPORT auto repr(unsigned short const& obj) -> ObjectInfo<unsigned short, Literal>;
EXPORT auto repr(signed int const& obj) -> ObjectInfo<signed int, Literal>;
EXPORT auto repr(unsigned int const& obj) -> ObjectInfo<unsigned int, Literal>;
EXPORT auto repr(signed long const& obj) -> ObjectInfo<signed long, Literal>;
EXPORT auto repr(unsigned long const& obj) -> ObjectInfo<unsigned long, Literal>;
EXPORT auto repr(signed long long const& obj) -> ObjectInfo<signed long long, Literal>;
EXPORT auto repr(unsigned long long const& obj) -> ObjectInfo<unsigned long long, Literal>;

// Floating-point types
EXPORT auto repr(float const& obj) -> ObjectInfo<float, Literal>;
EXPORT auto repr(double const& obj) -> ObjectInfo<double, Literal>;
EXPORT auto repr(long double const& obj) -> ObjectInfo<long double, Literal>;

// Character types
EXPORT auto repr(char const& obj) -> ObjectInfo<char, Literal>;

}  // namespace librepr