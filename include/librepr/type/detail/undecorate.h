#pragma once
#pragma comment(lib, "dbghelp.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DbgHelp.h>

#include <array>
#include <cassert>
#include <mutex>
#include <string>
#include <string_view>
#include <typeinfo>
#include <librepr/util/strings.h>

// Awful!! But simplifies the _DecoratedName -> Symbol conversion
#define M2B_CAST(b, m) [](auto* __ptr) { \
  return ((b*)((char*)(__ptr) - offsetof(b, m))); }

namespace librepr {
/// Checks if we need to use __FUNCDNAME__ or not.
/// As far as I know, it should always exist, but we have this just in case
inline constexpr bool msvc_rawname =
  requires(const std::type_info& ty) { ty.raw_name(); };

namespace detail {
/// Wrapped buffer for demangling
using DemangleBuffer = std::array<char, REPR_DEMANGLE_MAX>;

[[nodiscard]] inline std::string denoise_name(std::string_view name) {
  auto ret = std::string{name.data()};
  // TODO: Actually parse the undecorated symbols T-T
  remove_all(ret, "struct ");
  remove_all(ret, "class ");
  remove_all(ret, "enum ");
  return ret;
}

// TODO: Make this better ig
[[nodiscard]] inline int syminit_() {
  SymSetOptions(SYMOPT_ALLOW_ABSOLUTE_SYMBOLS | SYMOPT_DEFERRED_LOADS);
  bool did_initialize = !!SymInitialize(GetCurrentProcess(), NULL, TRUE);
  assert(did_initialize && "Failed to initialize the symbol handler!");
  return true;
}

inline std::size_t undecorate_name(const char* symdata, DemangleBuffer& buf) {
  { [[maybe_unused]] static int _ = syminit_(); }
  static std::mutex mtx { };
  if constexpr (msvc_rawname) {
    // It should go without saying but...
    // DON'T pass random strings starting with `.`
    if (*symdata == '.') [[likely]] {
      using uname_type = ::__std_type_info_data;
      symdata = M2B_CAST(uname_type, _DecoratedName)(
        symdata)->_UndecoratedName;
      if(!symdata) [[unlikely]] { return false; }
      auto denoised = denoise_name({ symdata });
      std::memcpy(buf.data(), denoised.data(), denoised.size());
      return denoised.size();
    }
  }
  std::scoped_lock lock { mtx };
  return std::size_t(::UnDecorateSymbolName(symdata, buf.data(),
    REPR_DEMANGLE_MAX, UNDNAME_NO_MS_KEYWORDS));
}

}  // namespace detail
}  // namespace librepr
