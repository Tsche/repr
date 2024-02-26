#pragma once
#include <librepr/feature.h>
#include "platform.h"
#include "warning.h"

#ifndef REPR_HARD_CHECKS
# define REPR_HARD_CHECKS OFF
#else
# undef REPR_HARD_CHECKS
// Redefining so it matches the conventions.
// This avoids cases where the user may do something
// like `-DREPR_HARD_CHECKS=1` failing.
# define REPR_HARD_CHECKS ON
#endif

#if !defined(REPR_RTTI) && !defined(REPR_CTTI)
    #define REPR_RTTI ON
    #define REPR_CTTI OFF
#elif defined(REPR_RTTI) && !defined(REPR_CTTI)
    #if USING(REPR_RTTI)
        #define REPR_CTTI OFF
    #else
        #define REPR_CTTI ON
    #endif
#elif !defined(REPR_RTTI) && defined(REPR_CTTI)
    #if USING(REPR_CTTI)
        #define REPR_RTTI OFF
    #else
        #define REPR_RTTI ON
    #endif
#else
    #if USING(REPR_RTTI) && USING(REPR_CTTI)
        #error "Select either RTTI or CTTI"
    #endif
#endif

#if !defined(REPR_SOURCE_LOCATION)
    #define REPR_SOURCE_LOCATION OFF
#endif

#if !defined(REPR_DEFAULT_TEMPLATE_ARGUMENTS)
  #define REPR_DEFAULT_TEMPLATE_ARGUMENTS OFF
#endif

#if !defined(REPR_DEMANGLE_MAX)
  #define REPR_DEMANGLE_MAX 1024
#endif

#if !defined(REPR_USE_MAGIC_ENUM)
  #define REPR_USE_MAGIC_ENUM OFF
#endif

#if !defined(REPR_ENUM_MIN_SIGNED)
  #define REPR_ENUM_MIN_SIGNED -128
#endif

#if !defined(REPR_ENUM_MAX_SIGNED)
  #define REPR_ENUM_MAX_SIGNED 127
#endif

#if !defined(REPR_ENUM_MIN_UNSIGNED)
  #define REPR_ENUM_MIN_UNSIGNED 0
#endif

#if !defined(REPR_ENUM_MAX_UNSIGNED)
  #define REPR_ENUM_MAX_UNSIGNED 255
#endif

#if !defined(REPR_ENUM_CHUNKSIZE)
  #define REPR_ENUM_CHUNKSIZE 255
#endif

#if !defined(REPR_ENUM_FAST_SEARCH)
#define REPR_ENUM_FAST_SEARCH ON
#elif USING(LIBREPR_COMPILER_GCC)
  #define REPR_ENUM_FAST_SEARCH ON
#elif USING(LIBREPR_COMPILER_MSVC)
  //TODO
  #define REPR_ENUM_FAST_SEARCH OFF
#endif

#if !defined(REPR_ENUM_RECURSIVE_SEARCH)
#define REPR_ENUM_RECURSIVE_SEARCH OFF
#endif