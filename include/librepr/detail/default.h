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

#if !defined(REPR_NAMESPACE)
    #define REPR_NAMESPACE OFF
#endif

#if USING(REPR_NAMESPACE)
    #define REPR_INLINE
#else 
    #define REPR_INLINE inline
#endif 

#if !defined(REPR_DEFAULT_TEMPLATE_ARGUMENTS)
  #define REPR_DEFAULT_TEMPLATE_ARGUMENTS OFF
#endif

#if !defined(REPR_DEMANGLE_MAX)
  #define REPR_DEMANGLE_MAX 1024
#endif
