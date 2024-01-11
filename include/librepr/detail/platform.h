#pragma once
#include <librepr/feature.h>
#include <utility>

#if defined(__clang__)
# define REPR_CLANG ON
#elif defined(__GNUC__)
# define REPR_GCC ON
#elif defined(_MSC_VER)
# define REPR_MSVC ON
#else
# error Unsupported compiler!
#endif

#if defined(_WIN32) || defined(__WIN32__)
# define REPR_WINDOWS ON
#else
# define REPR_WINDOWS OFF
#endif

#if !defined(REPR_CLANG)
# define REPR_CLANG OFF
#endif

#if !defined(REPR_GCC)
# define REPR_GCC OFF
#endif

#if !defined(REPR_MSVC)
# define REPR_MSVC OFF
#endif

#ifndef LIBREPR_DEBUG
#if USING(REPR_MSVC)
# if defined(_DEBUG) && !defined(NDEBUG)
#  define LIBREPR_DEBUG 1
# endif
#else
# if !defined(NDEBUG)
#  define LIBREPR_DEBUG 1
# endif
#endif
#endif // LIBREPR_DEBUG?

# if defined(_WIN32)
// _CRT_*_C_HEADER macros
#  include <vcruntime.h>
# endif
# include <assert.h>

// Underlying asserts
#if defined(_WIN32)
# define LIBREPR_UND_ASSERT_(...) \
  _assert((__VA_ARGS__), (__FILE__), unsigned(__LINE__))
/*
 * Expose the normal string assertion function.
 * Windows generally uses `_wassert` for their
 * `assert`, which we don't want.
 */
_CRT_BEGIN_C_HEADER
  __declspec(dllimport) void 
   __cdecl _assert( // NOLINT
    char const* _Message,
    char const* _File,
    unsigned    _Line
  );
_CRT_END_C_HEADER

#elif defined(__APPLE__)
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

#ifdef __cpp_lib_unreachable
# define LIBREPR_UNREACHABLE() std::unreachable()
#elif USING(REPR_MSVC)
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
