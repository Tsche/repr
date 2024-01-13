#pragma once
#include <librepr/feature.h>
#include <librepr/detail/platform.h>

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
#if defined(__clang__)
#if __clang_major__ >= 16
  // https://github.com/llvm/llvm-project/issues/68489
  #define REPR_ENUM_FAST_SEARCH OFF
#else
  //TODO verify this works with clang <= 15
  #define REPR_ENUM_FAST_SEARCH ON
#endif
#elif defined(__GNUC__)
  #define REPR_ENUM_FAST_SEARCH ON
#elif defined(_MSC_VER)
  //TODO
  #define REPR_ENUM_FAST_SEARCH OFF
#endif
#endif

#if !defined(REPR_ENUM_RECURSIVE_SEARCH)
#define REPR_ENUM_RECURSIVE_SEARCH OFF
#endif