#pragma once
#include <string>

namespace librepr {
std::string demangle(const char* mangled);

#if defined(_WIN32)
namespace detail {
std::string& denoise_name(std::string& name);
}
#endif
}  // namespace librepr