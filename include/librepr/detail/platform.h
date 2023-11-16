#pragma once
#include <librepr/feature.h>

#if defined(__clang__)
#define REPR_CLANG ON
#elif defined(__GNUC__)
#define REPR_GCC ON
#elif defined(_MSC_VER)
#define REPR_MSVC ON
#else
#error Unsupported compiler!
#endif

#if defined(_WIN32) || defined(__WIN32__)
#define REPR_WINDOWS ON
#else
#define REPR_WINDOWS OFF
#endif

#if !defined(REPR_CLANG)
#define REPR_CLANG OFF
#endif

#if !defined(REPR_GCC)
#define REPR_GCC OFF
#endif

#if !defined(REPR_MSVC)
#define REPR_MSVC OFF
#endif