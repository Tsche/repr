#pragma once
#include <string>

#include <librepr/detail/visibility.h>

namespace librepr {
EXPORT std::string repr(std::nullptr_t obj);
EXPORT std::string repr(char const* obj);
EXPORT std::string repr(void const* obj);

}  // namespace librepr
