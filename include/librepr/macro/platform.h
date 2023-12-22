#pragma once

namespace librepr {

enum class Platform : unsigned { Unknown, Linux, UNIX, Windows, macOS };
enum class Compiler : unsigned { Unknown, Clang, GCC, MSVC };

#if defined(_WIN32)
#define LIBREPR_PLATFORM_WINDOWS
constexpr inline auto platform = Platform::Windows;

#elif defined(__linux__)
#define LIBREPR_PLATFORM_LINUX
constexpr inline auto platform = Platform::Linux;

#elif defined(__APPLE__)
#define LIBREPR_PLATFORM_MACOS
constexpr inline auto platform = Platform::macOS;

#elif defined(unix) || defined(__unix__) || defined(__unix)
#define LIBREPR_PLATFORM_UNIX
constexpr inline auto platform = Platform::UNIX;

#else
#warning "Unsupported platform"
constexpr inline auto compiler = Platform::Unknown;
#endif

#if defined(__clang__)
#define LIBREPR_COMPILER_CLANG
constexpr inline auto compiler = Compiler::Clang;

#elif defined(__GNUC__) || defined(__GNUG__)
#define LIBREPR_COMPILER_GCC
constexpr inline auto compiler = Compiler::GCC;

#elif defined(_MSC_VER)
#define LIBREPR_COMPILER_MSVC
constexpr inline auto compiler = Compiler::MSVC;

#else
#warning "Unsupported compiler"
constexpr inline auto compiler = Compiler::Unknown;
#endif

#if defined(LIBREPR_COMPILER_MSVC)
#define LIBREPR_FUNCTION_NAME __FUNCSIG__
#else
#define LIBREPR_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

}  // namespace librepr