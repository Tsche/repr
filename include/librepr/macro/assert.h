#pragma once
#include <librepr/feature.h>
#include "platform.h"
#include "default.h"
#include "format.h"

#ifndef LIBREPR_DEBUG
# if USING(LIBREPR_COMPILER_MSVC)
#   if defined(_DEBUG) && !defined(NDEBUG)
#     define LIBREPR_DEBUG 1
#   endif
# else
#   if !defined(NDEBUG)
#     define LIBREPR_DEBUG 1
#   endif
# endif
#endif // LIBREPR_DEBUG?

#if USING(LIBREPR_PLATFORM_WINDOWS)
# if USING(LIBREPR_COMPILER_MSVC)
// _CRT_*_C_HEADER macros
#  include <vcruntime.h>
#  define LIBREPR_CRT_BEGIN_C _CRT_BEGIN_C_HEADER
#  define LIBREPR_CRT_END_C _CRT_END_C_HEADER
# else
#  define LIBREPR_CRT_BEGIN_C \
    _Pragma("pack(push, 8)") \
    extern "C" {
#  define LIBREPR_CRT_END_C } \
    _Pragma("pack(pop)")
# endif
#endif
#include <assert.h>

// Underlying asserts
#if USING(LIBREPR_PLATFORM_WINDOWS)
# define LIBREPR_UND_ASSERT_(...) \
  _assert((__VA_ARGS__), (__FILE__), unsigned(__LINE__))
/*
 * Expose the normal string assertion function.
 * Windows generally uses `_wassert` for their
 * `assert`, which we don't want.
 */
LIBREPR_CRT_BEGIN_C
  __declspec(dllimport) void 
   __cdecl _assert( // NOLINT
    char const* _Message,
    char const* _File,
    unsigned    _Line
  );
LIBREPR_CRT_END_C

#elif USING(LIBREPR_PLATFORM_MACOS)
# define LIBREPR_UND_ASSERT_(...) \
  __assert_rtn((__func__), (__FILE__), unsigned(__LINE__), (__VA_ARGS__))
#else
# define LIBREPR_UND_ASSERT_(...) \
  __assert_fail((__VA_ARGS__), __FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

// Exposing assertion functions
#ifdef LIBREPR_DEBUG
/// Explicit assertion, no condition checks. For fatal errors.
# define LIBREPR_EXASSERT(...) \
  LIBREPR_UND_ASSERT_((REPR_FORMAT(__VA_ARGS__)).c_str())
/// Checked assertion, for constraint enforcement.
# define LIBREPR_ASSERT(cond, ...) \
 do { if(!(cond)) [[unlikely]] { LIBREPR_EXASSERT(__VA_ARGS__); } } while(0)
#else
# define LIBREPR_EXASSERT(...) ::librepr::detail::unreachable()
# define LIBREPR_ASSERT(...) (void)(0)
#endif

#if USING(REPR_HARD_CHECKS)
# define LIBREPR_SOFT_ASSERT(...) LIBREPR_ASSERT(__VA_ARGS__)
#else
/// Only checks assertion when `REPR_HARD_CHECKS` is `ON`.
# define LIBREPR_SOFT_ASSERT(...) (void)(0)
#endif

#ifdef __cpp_lib_unreachable
# define LIBREPR_UNREACHABLE() std::unreachable()
#elif USING(LIBREPR_COMPILER_MSVC)
# define LIBREPR_UNREACHABLE() __assume(false)
#else
# define LIBREPR_UNREACHABLE() __builtin_unreachable()
#endif

namespace librepr::detail {
#ifndef __cpp_lib_unreachable
  [[noreturn]] inline void unreachable() {
    LIBREPR_UNREACHABLE();
  }
#else // Supports unreachable
  using std::unreachable;
#endif // __cpp_lib_unreachable
} // namespace librepr::detail