#pragma once

#ifndef __has_builtin
#define LIBREPR_HAS_BUILTIN(BUILTIN) 0
#else
#define LIBREPR_HAS_BUILTIN(BUILTIN) __has_builtin(BUILTIN)
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