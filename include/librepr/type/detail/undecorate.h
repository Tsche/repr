#pragma once
#pragma comment(lib, "dbghelp.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DbgHelp.h>

#include <array>
#include <mutex>
#include <typeinfo>

#include <librepr/macro/assert.h>

#include "denoise.h"
#include "undname.h"

namespace librepr::detail {
namespace msvc {
  /// Checks if we need to use __FUNCDNAME__ or not.
  /// As far as I know, it should always exist, but we have this just in case
  inline constexpr bool has_rawname =
    requires(const std::type_info& ty) { ty.raw_name(); };
}  // namespace msvc

// TODO: Make this better ig
[[nodiscard]] inline int syminit_() {
  SymSetOptions(SYMOPT_ALLOW_ABSOLUTE_SYMBOLS | SYMOPT_DEFERRED_LOADS);
  bool did_initialize = !!SymInitialize(GetCurrentProcess(), NULL, TRUE);
  assert(did_initialize && "Failed to initialize the symbol handler!");
  return true;
}

inline std::size_t undecorate_name(const char* symdata, DemangleBuffer& buf) {
  { [[maybe_unused]] static int sym = syminit_(); }
  static std::mutex mtx { };
  if(!symdata) [[unlikely]] { return 0; }
  if constexpr (msvc::has_rawname) {
    if (*symdata == '.') [[likely]] {
      static constexpr auto flags =
        UndStrategy::Decode32Bit | UndStrategy::TypeOnly;
      msvc::unDName((symdata + 1), buf, UndStrategy::Type(flags));
      auto denoised = denoise_name({ buf.data() });
      std::memcpy(buf.data(), denoised.data(), denoised.size());
      return denoised.size();
    }
  }
  LIBREPR_SOFT_ASSERT(*symdata == '?', 
    "Got '{}', expected '?'", *symdata);
  std::scoped_lock lock { mtx };
  return std::size_t(::UnDecorateSymbolName(symdata, buf.data(),
    REPR_DEMANGLE_MAX, UNDNAME_NO_MS_KEYWORDS));
}
}  // namespace librepr::detail
