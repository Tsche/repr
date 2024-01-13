#pragma once
#include <string>
#include <string_view>
#include <librepr/util/string.h>

#if defined _WIN32
#pragma comment(lib, "dbghelp.lib")
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <DbgHelp.h>
#include <array>

#else
#include <cxxabi.h>
#include <memory>
#endif

namespace librepr {

[[nodiscard]] inline std::string demangle(std::string_view mangled) {
#if defined _WIN32
  auto buffer = std::array<char, REPR_DEMANGLE_MAX>{};
  auto result = ::UnDecorateSymbolName(mangled.data(), buffer.data(), REPR_DEMANGLE_MAX,
                                       UNDNAME_NAME_ONLY | UNDNAME_NO_MS_KEYWORDS);
  if (not result) {
    // could not demangle
    return mangled.data();
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