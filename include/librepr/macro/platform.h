#pragma once
#include <librepr/feature.h>

namespace librepr {

enum class Platform : unsigned { Unknown, Linux, UNIX, Windows, macOS };
enum class Compiler : unsigned { Unknown, Clang, GCC, MSVC };

#if defined(_WIN32)
#define LIBREPR_PLATFORM_WINDOWS ON
constexpr inline auto platform = Platform::Windows;

#elif defined(__linux__)
#define LIBREPR_PLATFORM_LINUX ON
constexpr inline auto platform = Platform::Linux;

#elif defined(__APPLE__)
#define LIBREPR_PLATFORM_MACOS ON
constexpr inline auto platform = Platform::macOS;

#elif defined(unix) || defined(__unix__) || defined(__unix)
#define LIBREPR_PLATFORM_UNIX ON
constexpr inline auto platform = Platform::UNIX;

#else
#warning "Unsupported platform"
constexpr inline auto compiler = Platform::Unknown;
#endif

#if !defined(LIBREPR_PLATFORM_WINDOWS)
# define LIBREPR_PLATFORM_WINDOWS OFF
#endif

#if !defined(LIBREPR_PLATFORM_LINUX)
# define LIBREPR_PLATFORM_LINUX OFF
#endif

#if !defined(LIBREPR_PLATFORM_MACOS)
# define LIBREPR_PLATFORM_MACOS OFF
#endif

#if !defined(LIBREPR_PLATFORM_UNIX)
# define LIBREPR_PLATFORM_UNIX OFF
#endif


#if defined(__clang__)
#define LIBREPR_COMPILER_CLANG ON
constexpr inline auto compiler = Compiler::Clang;

#elif defined(__GNUC__) || defined(__GNUG__)
#define LIBREPR_COMPILER_GCC ON
constexpr inline auto compiler = Compiler::GCC;

#elif defined(_MSC_VER)
#define LIBREPR_COMPILER_MSVC ON
constexpr inline auto compiler = Compiler::MSVC;

#else
#warning "Unsupported compiler"
constexpr inline auto compiler = Compiler::Unknown;
#endif

#if !defined(LIBREPR_COMPILER_CLANG)
# define LIBREPR_COMPILER_CLANG OFF
#endif

#if !defined(LIBREPR_COMPILER_GCC)
# define LIBREPR_COMPILER_GCC OFF
#endif

#if !defined(LIBREPR_COMPILER_MSVC)
# define LIBREPR_COMPILER_MSVC OFF
#endif


#if USING(LIBREPR_COMPILER_MSVC)
#define LIBREPR_FUNCTION_NAME __FUNCSIG__
#else
#define LIBREPR_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

}  // namespace librepr