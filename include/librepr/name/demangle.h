#pragma once
#include <string>
#include <string_view>
#include <librepr/macro/util.h>
#include <librepr/macro/platform.h>

#if USING(LIBREPR_COMPILER_MSVC)
#include "detail/undecorate.h"
#include "detail/denoise.h"
#elif __has_include(<cxxabi.h>)
#include <cxxabi.h>
#include <memory>
#else
#error No available demangling method!
#endif

namespace librepr {
[[nodiscard]] inline std::string demangle(std::string_view mangled) {
#if USING(LIBREPR_COMPILER_MSVC)
  detail::DemangleBuffer buffer{};
  auto count = detail::undecorate_name(mangled.data(), buffer);
  if (count == 0) [[unlikely]] {
    // Demangling failed...
    return mangled.data();
  }
  if constexpr(!detail::msvc::has_rawname) {
    // Check if the symbol was obtained with __FUNCDNAME__
    if(mangled.starts_with("??$get_mangled_name")) [[likely]] {
      using namespace detail;
      std::string_view snip { buffer.data(), count };
      snip.remove_prefix(strsize("char const * librepr::get_mangled_name<"));
      snip.remove_suffix(strsize(">(void)"));
      return denoise_name(snip);
    }
  }
  return std::string(buffer.data(), count);
#else
  struct FreeDeleter {
    void operator()(void* ptr) const { std::free(ptr); }
  };

  auto const demangled = std::unique_ptr<char, FreeDeleter>{
    abi::__cxa_demangle(mangled.data(), nullptr, nullptr, nullptr)
  };

  return std::string{demangled ? demangled.get() : mangled};
#endif
}
}  // namespace librepr