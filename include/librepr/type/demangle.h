#pragma once
#include <string>
#include <string_view>
#include <librepr/detail/macros.h>
#include <librepr/detail/platform.h>

#if USING(REPR_MSVC)
#pragma comment(lib, "dbghelp.lib")
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <DbgHelp.h>
#include <librepr/util/strings.h>
#include <array>
#include <cassert>
#include <mutex>

#define REPR_MSC_XASSERT(...) \
  _assert((REPR_FORMAT(__VA_ARGS__).c_str()), (__FILE__), unsigned(__LINE__))
// Awful!! But simplifies the _DecoratedName -> Symbol conversion
#define M2B_CAST(b, m) [](auto* __ptr) { \
  return ((b*)((char*)(__ptr) - offsetof(b, m))); }

_CRT_BEGIN_C_HEADER
  __declspec(dllimport) void 
   __cdecl _assert( // NOLINT
    char const* _Message,
    char const* _File,
    unsigned    _Line
  );
_CRT_END_C_HEADER

#elif __has_include(<cxxabi.h>)
#include <cxxabi.h>
#include <memory>

#else
#error No available demangling method!
#endif

namespace librepr {

#if USING(REPR_MSVC)
using DemangleBuffer = std::array<char, REPR_DEMANGLE_MAX>;
namespace detail {
[[nodiscard]] inline std::string denoise_name(std::string_view name) {
  auto ret = std::string{name.data()};
  // TODO: Actually parse the undecorated symbols T-T
  remove_all(ret, "struct ");
  remove_all(ret, "class ");
  remove_all(ret, "enum ");
  return ret;
}

[[nodiscard]] inline int syminit_() {
  SymSetOptions(SYMOPT_ALLOW_ABSOLUTE_SYMBOLS | SYMOPT_DEFERRED_LOADS);
  bool did_initialize = !!SymInitialize(
    GetCurrentProcess(), NULL, TRUE);
  assert(did_initialize && "Failed to initialize the symbol handler!");
  return true;
}

inline bool undecorate_name(const char* symdata, DemangleBuffer& buf) {
  { [[maybe_unused]] static int _ = syminit_(); }
  static std::mutex mtx { };
  if (*symdata == '.') {
    symdata = M2B_CAST(::__std_type_info_data, _DecoratedName)(
      symdata)->_UndecoratedName;
    if(!symdata) [[unlikely]] { return false; }
    auto denoised = denoise_name({ symdata });
    std::memcpy(buf.data(), denoised.data(), denoised.size());
    return true;
  } else if (*symdata != '?') [[unlikely]] {
    // TODO: Remove this
    REPR_MSC_XASSERT("Invalid symbol `{}'", symdata);
  }
  std::scoped_lock lock { mtx };
  return (::UnDecorateSymbolName(symdata, buf.data(),
    REPR_DEMANGLE_MAX, UNDNAME_NAME_ONLY | UNDNAME_NO_MS_KEYWORDS) > 0);
}
}  // namespace detail
#endif

[[nodiscard]] inline std::string demangle(std::string_view mangled) {
#if USING(REPR_MSVC)
  DemangleBuffer buffer{};
  if (not detail::undecorate_name(mangled.data(), buffer)) {
    // could not demangle
    return std::string(mangled);
  }
  return std::string{buffer.data()};
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
}