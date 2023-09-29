#pragma once
#include <type_traits>
#include <format>
#include <string>

#include <librepr/detail/visibility.h>
#include <librepr/reflection/name.h>

namespace librepr {
template <typename T>
std::string get_name();

EXPORT std::string repr(std::nullptr_t obj);
EXPORT std::string repr(char const* obj);
EXPORT std::string repr(void const* obj);

}  // namespace librepr
