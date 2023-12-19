#pragma once
#include "macros.h"
#include "platform.h"

#if defined(LIBREPR_COMPILER_MSVC)

#define LIBREPR_WARNING_DISABLE_MSVC(WARNING) LIBREPR_PRAGMA(warning(disable: WARNING))
#define LIBREPR_WARNING_PUSH LIBREPR_PRAGMA(warning(push))
#define LIBREPR_WARNING_POP LIBREPR_PRAGMA(warning(pop))

#else
#define LIBREPR_WARNING_DISABLE_MSVC(...)
#endif

#if defined(LIBREPR_COMPILER_CLANG)

#define LIBREPR_WARNING_DISABLE_CLANG(WARNING) LIBREPR_PRAGMA(clang diagnostic ignored #WARNING)
#define LIBREPR_WARNING_PUSH LIBREPR_PRAGMA(clang diagnostic push)
#define LIBREPR_WARNING_POP LIBREPR_PRAGMA(clang diagnostic pop)

#else
#define LIBREPR_WARNING_DISABLE_CLANG(...)
#endif

#if defined(LIBREPR_COMPILER_GCC)

#define LIBREPR_WARNING_DISABLE_GCC(WARNING) LIBREPR_PRAGMA(GCC diagnostic ignored #WARNING)
#define LIBREPR_WARNING_PUSH LIBREPR_PRAGMA(GCC diagnostic push)
#define LIBREPR_WARNING_POP LIBREPR_PRAGMA(GCC diagnostic pop)

#else
#define LIBREPR_WARNING_DISABLE_GCC(...)
#endif

#define LIBREPR_WARNING_DISABLE(COMPILER, WARNING) LIBREPR_WARNING_DISABLE_##COMPILER(WARNING)
