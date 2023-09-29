#pragma once
#include <string>

#include <librepr/detail/visibility.h>


namespace librepr {
EXPORT std::string demangle(const char* mangled);

#if defined(_WIN32)
namespace detail {
std::string& denoise_name(std::string& name);
}
#endif
}  // namespace librepr