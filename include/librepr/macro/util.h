#pragma once
#include <librepr/feature.h>
#include "platform.h"

#ifndef __has_attribute
# define __has_attribute(x) (0)
#endif

#ifndef __has_builtin
#define LIBREPR_HAS_BUILTIN(BUILTIN) (0)
#else
#define LIBREPR_HAS_BUILTIN(BUILTIN) __has_builtin(BUILTIN)
#endif

#ifndef __has_builtin
# define __has_builtin(x) (0)
#endif

#ifndef __has_include
# define __has_include(x) (0)
#endif

#ifdef __has_cpp_attribute
# define REPR_HAS_CPP_ATTR(x) __has_cpp_attribute(x)
#else
# define REPR_HAS_CPP_ATTR(x) (0)
#endif

// Inlining macros
#if USING(LIBREPR_COMPILER_GCC) || USING(LIBREPR_COMPILER_CLANG)
# define LIBREPR_PROPAGATE_INLINE __attribute__(( \
   always_inline, flatten, artificial)) inline
# define LIBREPR_FORCE_INLINE __attribute__(( \
   always_inline, artificial)) inline
# define LIBREPR_NOINLINE __attribute__((noinline))
#elif USING(LIBREPR_COMPILER_MSVC)
# define LIBREPR_FORCE_INLINE __forceinline
# define LIBREPR_HINT_INLINE __forceinline
# define LIBREPR_NOINLINE __declspec(noinline)
#endif

#ifndef LIBREPR_PROPAGATE_INLINE
# define LIBREPR_PROPAGATE_INLINE LIBREPR_FORCE_INLINE
#endif

#ifndef LIBREPR_FORCE_INLINE
# define LIBREPR_FORCE_INLINE inline
#endif

#ifndef LIBREPR_HINT_INLINE
# define LIBREPR_HINT_INLINE inline
#endif

#ifndef LIBREPR_NOINLINE
# define LIBREPR_NOINLINE
#endif

#if __has_attribute(nodebug)
# define LIBREPR_NODEBUG __attribute__((nodebug))
#else
# define LIBREPR_NODEBUG
#endif

#if REPR_HAS_CPP_ATTR(gnu::hot) \
  && REPR_HAS_CPP_ATTR(gnu::cold)
# define LIBREPR_HOT [[gnu::hot]]
# define LIBREPR_COLD [[gnu::cold]]
#elif defined(__GNUC__)
# define LIBREPR_HOT __attribute__((hot))
# define LIBREPR_COLD __attribute__((cold))
#else
# define LIBREPR_HOT
# define LIBREPR_COLD
#endif

#if USING(LIBREPR_COMPILER_MSVC)
# define LIBREPR_EMPTY_BASES __declspec(empty_bases)
#else
# define LIBREPR_EMPTY_BASES
#endif

#if USING(LIBREPR_COMPILER_CLANG) || USING(LIBREPR_COMPILER_MSVC)
# define LIBREPR_RESTRICT __restrict
#elif USING(LIBREPR_COMPILER_GCC)
# define LIBREPR_RESTRICT __restrict__
#else
# define LIBREPR_RESTRICT
#endif


#define LIBREPR_PRAGMA(...) _Pragma(#__VA_ARGS__)

// clang-format 18 introduces AllowShortCompoundRequirementOnASingleLine which
// should fix the formatting issue with this macro. Disable clang-format for now
// clang-format off
#define LIBREPR_MAYBE_DO(...)                     \
  if constexpr (requires { { __VA_ARGS__ }; }) {  \
    __VA_ARGS__;                                  \
  }
// clang-format on